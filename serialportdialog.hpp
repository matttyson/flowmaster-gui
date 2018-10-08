#ifndef SERIALPORTDIALOG_HPP
#define SERIALPORTDIALOG_HPP

#include <QDialog>

class QPushButton;
class QComboBox;
class QLineEdit;

class SerialPortDialog : public QDialog
{
	Q_OBJECT
public:
	SerialPortDialog();
	~SerialPortDialog();

public slots:
	void okClick();
	void cancelClick();
	void testClick();

private:
	QPushButton *m_ok;
	QPushButton *m_cancel;
	QPushButton *m_testPort;
	QComboBox *m_baudRate;
#if defined(FM_WIN)
	QComboBox *m_serialPort;
#elif defined (FM_LINUX)
	QLineEdit *m_serialPort;
#endif
	void initLayouts();
	void loadSettings();
	void saveSettings();

	void initSerialPorts();
};

#endif // SERIALPORTDIALOG_HPP
