#ifndef FLOWMASTER_HPP
#define FLOWMASTER_HPP

#include <QWidget>
#include <QVector>
#include <QTimer>


class QwtThermo;
class QwtSlider;
class MtThermo;
class MtTachoMeter;
class QCheckBox;
class QLabel;

class QThread;

class FlowState;
class Monitor;

class gpu_api;
class MtClickableLabel;

class FlowmasterWidget : public QWidget
{
	Q_OBJECT
public:
	explicit FlowmasterWidget(QWidget *parent = 0);
	~FlowmasterWidget();

	void disconnectSerial();
	void connectSerial();

	void startMonitor();
	void stopMonitor();

	bool isConnected();


signals:

	void shutdownThread();
	void statusMessage(const QString &);

public slots:

	void fmStatus(const FlowState &);

	void fanChange(double value);
	void pumpChange(double value);

	void fanSliderReleased();
	void pumpSliderReleased();

	void pumpSliderPressed();
	void fanSliderPressed();

	void updateGpu();

	void regulateCheckBoxClick(int state);
	void labelClicked();

private:
	MtTachoMeter *m_fan_rpm;
	MtTachoMeter *m_pump_rpm;

	QwtSlider *m_fan_dcycle;
	QwtSlider *m_pump_dcycle;

	MtThermo *m_coolant_temp;
	MtThermo *m_ambient_temp;

	QCheckBox *m_autoRegulateCheck;
	MtClickableLabel *m_regulateLabel;

	QVector<MtThermo*> m_gpus;

	Monitor *m_fmMonitor;

	bool m_suspendFanUpdates;
	bool m_suspendPumpUpdates;
	bool m_autoRegulate;

	gpu_api *m_gpu;
	QTimer *gpu_timer;
private:

	void init_timer();
	void init_controls();
	void configure_thermo(QwtThermo *thermo);
};

#endif // FLOWMASTER_HPP
