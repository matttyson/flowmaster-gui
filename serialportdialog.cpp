#include "serialportdialog.hpp"

#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSettings>

#include "defines.h"

SerialPortDialog::SerialPortDialog()
{
	initSerialPorts();
	initLayouts();
	loadSettings();
}

SerialPortDialog::~SerialPortDialog()
{

}

void SerialPortDialog::initLayouts()
{
	m_baudRate = new QComboBox();
	m_baudRate->addItem("19200",19200);

	m_ok = new QPushButton("Ok");
	m_cancel = new QPushButton("Cancel");
	m_testPort = new QPushButton("Test");
	m_testPort->setDisabled(true);

	connect(m_ok,SIGNAL(clicked()),
			this, SLOT(okClick()));

	connect(m_cancel, SIGNAL(clicked()),
			this,SLOT(reject()));

	connect(m_testPort, SIGNAL(clicked()),
			this,SLOT(testClick()));

	QHBoxLayout *portLayout = new QHBoxLayout();

	portLayout->addWidget(new QLabel("Serial Port"));
	portLayout->addWidget(m_serialPort);

	QHBoxLayout *baudLayout = new QHBoxLayout();

	baudLayout->addWidget(new QLabel("Baud Rate"));
	baudLayout->addWidget(m_baudRate);

	QHBoxLayout *buttonLayout = new QHBoxLayout();
	buttonLayout->addWidget(m_testPort);;
	buttonLayout->addStretch();
	buttonLayout->addWidget(m_cancel);
	buttonLayout->addWidget(m_ok);

	QVBoxLayout *layout = new QVBoxLayout();

	layout->addLayout(portLayout);
	layout->addLayout(baudLayout);
	layout->addStretch();
	layout->addLayout(buttonLayout);

	this->setLayout(layout);
}

void SerialPortDialog::loadSettings()
{
	QSettings settings;

#ifdef _WIN32
	const QVariant itemData = settings.value(QS_SETTING_PORT);
	const int portIdx = m_serialPort->findData(itemData);
	if(portIdx != -1){
		m_serialPort->setCurrentIndex(portIdx);
	}
#else
	m_serialPort->setText(settings.value(QS_SETTING_PORT).toString());
#endif

	const int rate = settings.value(QS_SETTING_BAUD).toInt();

	const int idx = m_baudRate->findData(rate);
	if(idx != -1){
		m_baudRate->setCurrentIndex(idx);
	}
}

void SerialPortDialog::saveSettings()
{
	QSettings settings;
#ifdef _WIN32
	const int idx = m_serialPort->currentIndex();
	settings.setValue(QS_SETTING_PORT, m_serialPort->itemData(idx));
#else
	settings.setValue(QS_SETTING_PORT, m_serialPort->text());
#endif
	settings.setValue(QS_SETTING_BAUD, m_baudRate->itemData(m_baudRate->currentIndex()));
}

void SerialPortDialog::okClick()
{
	saveSettings();
	accept();
}

void SerialPortDialog::cancelClick()
{
	reject();
}

void SerialPortDialog::testClick()
{

}
