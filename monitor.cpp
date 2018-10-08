#include "monitor.hpp"
#include "flowmaster2.hpp"
#include "flowstate.hpp"

#include <QTimer>

#include <memory>

#ifdef _WIN32
#include <string>
#endif

Monitor::Monitor(const QString &port,QObject *parent) :
	QThread(parent),m_port(port),m_continue(true)
{
}

Monitor::~Monitor()
{

}



void Monitor::run()
{
	auto m_flowmaster = std::make_unique<flowmaster>();

#ifdef _WIN32
	const std::string str = m_port.toStdString();
	const char *port = str.c_str();
#else
	const char *port = m_port.toLocal8Bit().constData();
#endif
	int rc = m_flowmaster->connect(port);

	if(rc != FM_OK){
		// emit an error

		return;
	}

	while(m_continue){

		m_mutex.lock();
		foreach(const Command &c, m_commands){
			switch(c.command){
			case Cmd::FAN_SPEED:
				m_flowmaster->set_fan_speed(c.float_value);
				break;
			case Cmd::PUMP_SPEED:
				m_flowmaster->set_pump_speed(c.float_value);
				break;
			case Cmd::AUTOREGULATE:
				m_flowmaster->autoregulate(c.bool_value);
				break;
			}
		}
		m_commands.clear();
		m_mutex.unlock();

		const int rc = m_flowmaster->update_status();

		if(rc == 0){

		const auto pumpDc = m_flowmaster->pump_duty_cycle();
		const auto fanDc = m_flowmaster->fan_duty_cycle();

		const auto coolantTemp = m_flowmaster->coolant_temp();
		const auto ambientTemp = m_flowmaster->ambient_temp();

		const auto flowRate = 0.0f;

		const auto fanRpm = m_flowmaster->fan_rpm();
		const auto pumpRpm = m_flowmaster->pump_rpm();

		const time_t timestamp = time(NULL);

		FlowState state(pumpDc,fanDc,coolantTemp,ambientTemp,flowRate,fanRpm,pumpRpm,timestamp);

		emit status(state);
		}

		sleep(1);
	}

	m_flowmaster->disconnect();
}

void Monitor::setFanSpeed(float speed)
{
	queue(Command(Cmd::FAN_SPEED, speed));
}

void Monitor::setPumpSpeed(float speed)
{
	queue(Command(Cmd::PUMP_SPEED, speed));
}

void Monitor::autoRegulate(bool enabled)
{
	queue(Command(Cmd::AUTOREGULATE, enabled));
}

void Monitor::stop()
{
	m_continue = false;
}

void Monitor::queue(const Command &cmd)
{
	m_mutex.lock();
	m_commands.append(cmd);
	m_mutex.unlock();
}
