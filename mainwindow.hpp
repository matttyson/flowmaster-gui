#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class FlowmasterWidget;
class QAction;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void setupSerialPort();
	void connectSerial();
	void disconnectSerial();
	void setStatusMessage(const QString &message);

	void changeEvent(QEvent *event) override;
private:
	FlowmasterWidget *m_flowmaster;
	QAction *m_connect;
	QAction *m_disconnect;

	void setupMenubar();
	void setupStatusbar();
};

#endif // MAINWINDOW_HPP
