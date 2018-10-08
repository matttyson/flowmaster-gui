#ifndef FlowState_HPP
#define FlowState_HPP

#include <ctime>
#include <QMetaType>


class FlowState
{
public:
	FlowState(float pumpDutyCycle,
			float fanDutyCycle,
			float coolantTemp,
			float ambientTemp,
			float flowRate,
			int fanRpm,
			int pumpRpm,
			time_t timestamp);
	FlowState();

	float pumpDutyCycle()const;
	float fanDutyCycle()const;
	float coolantTemp()const;
	float ambientTemp()const;
	int fanRpm()const;
	int pumpRpm()const;
	time_t timestamp()const;

private:
	float m_pumpDutyCycle;
	float m_fanDutyCycle;

	float m_coolantTemp;
	float m_ambientTemp;

	float m_flowRate;

	int m_fanRpm;
	int m_pumpRpm;

	time_t m_timestamp;
};

Q_DECLARE_METATYPE(FlowState)


#endif // FlowState_HPP
