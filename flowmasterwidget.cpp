#include "flowmasterwidget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QThread>
#include <QMetaType>
#include <QSettings>
#include <QCheckBox>
#include <QLabel>

#include <qwt_slider.h>
#include <qwt_thermo.h>
#include <qwt_slider.h>

#include "defines.h"

#include "flowstate.hpp"
#include "tachometer.hpp"
#include "mtthermo.hpp"
#include "mtclickablelabel.hpp"
#include "monitor.hpp"
#include "gpu_api.hpp"

FlowmasterWidget::FlowmasterWidget(QWidget *parent) :
	QWidget(parent),m_fmMonitor(nullptr), m_gpu(gpu_api::create_instance())
{
	init_controls();
	startMonitor();

	m_suspendFanUpdates = false;
	m_suspendPumpUpdates = false;
	m_autoRegulate = true;

	init_timer();
}

FlowmasterWidget::~FlowmasterWidget()
{
	if(m_fmMonitor != nullptr){
		m_fmMonitor->stop();
		m_fmMonitor->wait();
		delete m_fmMonitor;
	}

	delete m_gpu;
}

void FlowmasterWidget::stopMonitor()
{
	if(m_fmMonitor != nullptr){
		m_fmMonitor->stop();
		m_fmMonitor->wait();
		delete m_fmMonitor;
		m_fmMonitor = nullptr;
	}

	m_fan_rpm->setValue(0.0);
	m_pump_rpm->setValue(0.0);

	m_fan_dcycle->setValue(0.0);
	m_pump_dcycle->setValue(0.0);

	m_coolant_temp->setValue(0.0);
	m_ambient_temp->setValue(0.0);
}

void FlowmasterWidget::startMonitor()
{
	if(m_fmMonitor != nullptr){
		return;
	}

	QSettings settings;

	const QString port = settings.value(QS_SETTING_PORT).toString();

/*
	Insert some code to test the connection before we start monitoring.
*/

	m_fmMonitor = new Monitor(port);

	connect(m_fmMonitor,SIGNAL(status(FlowState)),
			this,SLOT(fmStatus(FlowState)));

	m_fmMonitor->start();
}

void FlowmasterWidget::init_controls()
{
	QHBoxLayout *layout = new QHBoxLayout;

	/* Duty cycle widgets */
	m_fan_dcycle = new QwtSlider(Qt::Vertical,this);
	m_pump_dcycle = new QwtSlider(Qt::Vertical,this);

	m_fan_dcycle->setScale(0.0,100.0);
	m_pump_dcycle->setScale(0.0,100.0);

	connect(m_fan_dcycle,SIGNAL(sliderPressed()),
			this,SLOT(fanSliderPressed()));
	connect(m_pump_dcycle,SIGNAL(sliderPressed()),
			this,SLOT(pumpSliderPressed()));

	connect(m_fan_dcycle,SIGNAL(sliderReleased()),
			this,SLOT(fanSliderReleased()));
	connect(m_pump_dcycle,SIGNAL(sliderReleased()),
			this,SLOT(pumpSliderReleased()));

	QGroupBox *dcycleGroup = new QGroupBox("Duty Cycle");

	QVBoxLayout *dcycle_layout = new QVBoxLayout;

	QHBoxLayout *horiz = new QHBoxLayout;
	m_autoRegulateCheck = new QCheckBox;

	connect(m_autoRegulateCheck, SIGNAL(stateChanged(int)),
			this, SLOT(regulateCheckBoxClick(int)));

	// TODO: Make the label clickable
	m_regulateLabel = new MtClickableLabel("Automatic Control");
	m_regulateLabel->setAlignment(Qt::AlignLeft);
	horiz->addWidget(m_autoRegulateCheck);
	horiz->addWidget(m_regulateLabel);
	horiz->addStretch();
	m_autoRegulateCheck->setCheckState(Qt::Checked);
	regulateCheckBoxClick(m_autoRegulateCheck->checkState());

	connect(m_regulateLabel, SIGNAL(clicked()),
			this, SLOT(labelClicked()));

	dcycle_layout->addLayout(horiz);

	horiz = new QHBoxLayout;

	QVBoxLayout *vert = new QVBoxLayout;

	QLabel *label = new QLabel("Fan");
	label->setAlignment(Qt::AlignHCenter);
	vert->addWidget(label);
	vert->addWidget(m_fan_dcycle);
	horiz->addLayout(vert);

	vert = new QVBoxLayout;
	label = new QLabel("Pump");
	label->setAlignment(Qt::AlignHCenter);
	vert->addWidget(label);
	vert->addWidget(m_pump_dcycle);
	horiz->addLayout(vert);

	dcycle_layout->addLayout(horiz);

	dcycleGroup->setLayout(dcycle_layout);
	layout->addWidget(dcycleGroup);

	QGroupBox *tempGroups = new QGroupBox("Temperature");
	QVBoxLayout *tempLayout = new QVBoxLayout;

	/* coolant / ambient temps */
	m_coolant_temp = new MtThermo(Qt::Vertical,"Coolant",this);
	m_ambient_temp = new MtThermo(Qt::Vertical,"Ambient",this);

	tempLayout->addWidget(m_coolant_temp);
	tempLayout->addWidget(m_ambient_temp);

	tempGroups->setLayout(tempLayout);
	layout->addWidget(tempGroups);

	if(m_gpu != nullptr){
		// GPU TEMPS
		for(int i = 0; i < m_gpu->gpu_count(); i++) {
			MtThermo *thermo = new MtThermo(Qt::Vertical, QString("GPU %1").arg(i), this);
			thermo->setScale(0.0, 100.0);
			thermo->setColorMap(0.0, 70.0, 90.0);
			m_gpus.append(thermo);
		}
		const int max_rows = 2;
		QGridLayout *gpuTempLayout = new QGridLayout;
		int row = 0;
		int col = 0;
		foreach(MtThermo *gpu, m_gpus) {
			gpuTempLayout->addWidget(gpu, row, col);
			row ++;
			if(row == max_rows) {
				row = 0;
				col++;
			}
		}
		QGroupBox *gpuTempBox = new QGroupBox("GPU Temp");
		gpuTempBox->setLayout(gpuTempLayout);
		layout->addWidget(gpuTempBox);
	}

	/* Pump and fan speed tachos */
	m_fan_rpm = new MtTachoMeter;
	m_pump_rpm = new MtTachoMeter;

	m_fan_rpm->setString("Fans");
	m_pump_rpm->setString("Pump");

	m_fan_rpm->setScale(0.0,2200.0);
	m_fan_rpm->setScaleStepSize(200);
	m_pump_rpm->setScale(0.0,5000.0);
	m_pump_rpm->setScaleStepSize(500);

	QGroupBox *tachoGroup = new QGroupBox("Tacho");
	QVBoxLayout *tachos = new QVBoxLayout;
	tachos->addWidget(m_fan_rpm);
	tachos->addWidget(m_pump_rpm);

	tachoGroup->setLayout(tachos);
	layout->addWidget(tachoGroup);

	setLayout(layout);
}


void FlowmasterWidget::configure_thermo(QwtThermo *thermo)
{
	thermo->setFont(QFont("Helvecta",8));
	thermo->setScale(0.0,80.0);
	thermo->setPipeWidth(6);
	thermo->setScaleMaxMajor(6);
	thermo->setScaleMaxMinor(5);

	thermo->setFillBrush(Qt::darkMagenta);
}

void FlowmasterWidget::fmStatus(const FlowState &state)
{
	if(!m_suspendPumpUpdates){
		m_pump_dcycle->setValue(state.pumpDutyCycle() * 100.0);
	}
	if(!m_suspendFanUpdates){
		m_fan_dcycle->setValue(state.fanDutyCycle() * 100.0);
	}

	m_coolant_temp->setValue(state.coolantTemp());
	m_ambient_temp->setValue(state.ambientTemp());

	m_fan_rpm->setValue(state.fanRpm());
	m_pump_rpm->setValue(state.pumpRpm());

	const QString message = QString("Coolant: %1 Ambient: %2")
			.arg(QString::number(state.coolantTemp(),'f',1))
			.arg(QString::number(state.ambientTemp(),'f',1));
	emit statusMessage(message);
}

void FlowmasterWidget::init_timer()
{
	gpu_timer = new QTimer(this);
	connect(gpu_timer, SIGNAL(timeout()), this, SLOT(updateGpu()));
	gpu_timer->start(1000);
}

void FlowmasterWidget::updateGpu()
{
	for(int i = 0; i < m_gpus.size(); i++) {
		const int temp = m_gpu->gpu_core_temp(i);
		m_gpus.at(i)->setValue(temp);
	}
}

void FlowmasterWidget::fanChange(double value)
{
	qDebug("Value is %f",value);
}

void FlowmasterWidget::pumpChange(double value)
{
	qDebug("Value is %f",value);
}

void FlowmasterWidget::fanSliderPressed()
{
	m_suspendFanUpdates = true;
}

void FlowmasterWidget::pumpSliderPressed()
{
	m_suspendPumpUpdates = true;
}

void FlowmasterWidget::fanSliderReleased()
{
	if(m_fmMonitor != nullptr){
		const auto value = m_fan_dcycle->value();
		m_fmMonitor->setFanSpeed(value / 100.0);
	}
	m_suspendFanUpdates = false;
}

void FlowmasterWidget::pumpSliderReleased()
{
	if(m_fmMonitor != nullptr){
		auto value = m_pump_dcycle->value();
		if(value < 20.0){
			value = 20.0;
		}
		m_fmMonitor->setPumpSpeed(value / 100.0);
	}
	m_suspendPumpUpdates = false;
}

bool FlowmasterWidget::isConnected()
{
	return true;
}

void FlowmasterWidget::regulateCheckBoxClick(int state)
{
	m_autoRegulate = state == Qt::Checked;

	m_fan_dcycle->setEnabled(!m_autoRegulate);
	m_pump_dcycle->setEnabled(!m_autoRegulate);

	if(m_fmMonitor != nullptr){
		m_fmMonitor->autoRegulate(m_autoRegulate);
	}
	// Trigger a redraw, shouldn't need to do this.
	// Something wrong with QwtSlider ?
	m_fan_dcycle->update();
	m_pump_dcycle->update();
}

void FlowmasterWidget::labelClicked()
{
	auto state = m_autoRegulateCheck->checkState();

	switch(state){
	case Qt::Unchecked:
		m_autoRegulateCheck->setCheckState(Qt::Checked);
		break;
	case Qt::Checked:
		m_autoRegulateCheck->setCheckState(Qt::Unchecked);
		break;
	case Qt::PartiallyChecked:
		// Shutup you stupid compiler
		break;
	}
}
