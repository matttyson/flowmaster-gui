#include "mainwindow.hpp"

#include "flowmasterwidget.hpp"
#include "serialportdialog.hpp"

#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QEvent>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setupMenubar();
	setupStatusbar();

	m_flowmaster = new FlowmasterWidget(this);
	setCentralWidget(m_flowmaster);

	connect(m_flowmaster,SIGNAL(statusMessage(QString)),
			this,SLOT(setStatusMessage(QString)));

}

MainWindow::~MainWindow()
{
}


void MainWindow::setupMenubar()
{
	QMenuBar *bar = new QMenuBar(this);

	QMenu *file_menu = new QMenu("File");
	bar->addMenu(file_menu);

	QAction *action = file_menu->addAction("Serial Port");

	connect(action,SIGNAL(triggered()),
			this, SLOT(setupSerialPort()));


	file_menu->addSeparator ();

	m_connect = file_menu->addAction("Connect");
	connect(m_connect, SIGNAL(triggered()),
			this, SLOT(connectSerial()));

	m_disconnect = file_menu->addAction("Disconnect");
	connect(m_disconnect, SIGNAL(triggered()),
			this,SLOT(disconnectSerial()));

	file_menu->addSeparator();

	action = file_menu->addAction("Exit");

	setMenuBar(bar);

}

void MainWindow::setupStatusbar()
{
	QStatusBar *statusBar = new QStatusBar;
	setStatusBar(statusBar);
}

void MainWindow::setStatusMessage(const QString &message)
{
	this->statusBar()->showMessage(message);
}

void MainWindow::setupSerialPort()
{
	SerialPortDialog dlg;

	int rc = dlg.exec();

	if(rc == QDialog::Accepted){
		/* need to trigger a reconnection here*/
	}
}

void MainWindow::connectSerial()
{
	m_flowmaster->startMonitor();
	m_connect->setEnabled(false);
	m_disconnect->setEnabled(true);
}

void MainWindow::disconnectSerial()
{
	m_flowmaster->stopMonitor();
	m_connect->setEnabled(true);
	m_disconnect->setEnabled(false);
}

void MainWindow::changeEvent(QEvent *event)
{
	if(event->type() == QEvent::WindowStateChange){
		if(isMinimized()){
			hide();
			event->ignore();
		}
	}
}
