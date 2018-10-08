#include "mainwindow.hpp"
#include <QApplication>

#include "defines.h"
#include <QMetaType>
#include "flowstate.hpp"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QCoreApplication::setOrganizationDomain(QS_ORG);
	QCoreApplication::setApplicationName(QS_APP);
	QCoreApplication::setOrganizationDomain(QS_DOMAIN);

	qRegisterMetaType<FlowState>("FlowState");

	MainWindow w;
	w.show();

	return a.exec();
}
