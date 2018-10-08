

#include "serialportdialog.hpp"

#include "enumser.h"

#include <vector>
#include <string>

#include <QString>
#include <QComboBox>
#include <QDebug>

void SerialPortDialog::initSerialPorts()
{
	std::vector<std::wstring> ports;

	m_serialPort = new QComboBox;

	if(!CEnumerateSerial::UsingRegistry(ports)){
		// oops
		return;
	}

	for(const auto &it: ports){
		const QString str = QString::fromStdWString(it);
		m_serialPort->addItem(str,str);
	}
}
