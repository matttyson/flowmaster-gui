#ifndef MONITOR_HPP
#define MONITOR_HPP


#include <QThread>
#include <QString>
#include <QMutex>
#include "flowstate.hpp"

/*
	This should be run in a background thread so that delays in serial comms don't
	block the gui.
*/

enum class Cmd {
	FAN_SPEED,
	PUMP_SPEED,
	AUTOREGULATE,
};

struct Command
{
	Command(Cmd cd, float value)
		:command(cd),float_value(value){}
	Command(Cmd cd, bool value)
		:command(cd),bool_value(value){}

	Cmd command;
	union {
		float float_value;
		bool bool_value;
	};
};

class Monitor : public QThread
{
	Q_OBJECT
public:
	explicit Monitor(const QString &port, QObject *parent = 0);
	~Monitor();

	void run() override;

signals:

	void status(const FlowState &);
	void finished();

public slots:

	void stop();
	void setFanSpeed(float speed);
	void setPumpSpeed(float speed);
	void autoRegulate(bool enabled);

private:
	QMutex m_mutex;
	QList<Command> m_commands;
	QString m_port;

	volatile bool m_continue;
	void queue(const Command &cmd);

};

#endif // MONITOR_HPP
