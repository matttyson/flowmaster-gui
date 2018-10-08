#include "flowstate.hpp"

FlowState::FlowState(float pumpDutyCycle,
				 float fanDutyCycle,
				 float coolantTemp,
				 float ambientTemp,
				 float flowRate,
				 int fanRpm,
				 int pumpRpm,
				 time_t timestamp)
	:m_pumpDutyCycle(pumpDutyCycle),
	  m_fanDutyCycle(fanDutyCycle),
	  m_coolantTemp(coolantTemp),
	  m_ambientTemp(ambientTemp),
	  m_flowRate(flowRate),
	  m_fanRpm(fanRpm),
	  m_pumpRpm(pumpRpm),
	  m_timestamp(timestamp)
{
}

FlowState::FlowState()
	:m_pumpDutyCycle(0),
	  m_fanDutyCycle(0),
	  m_coolantTemp(0),
	  m_ambientTemp(0),
	  m_flowRate(0),
	  m_fanRpm(0),
	  m_pumpRpm(0),
	  m_timestamp(0)
{
}

float FlowState::pumpDutyCycle()const
{
	return m_pumpDutyCycle;
}

float FlowState::fanDutyCycle()const
{
	return m_fanDutyCycle;
}

float FlowState::coolantTemp()const
{
	return m_coolantTemp;
}

float FlowState::ambientTemp()const
{
	return m_ambientTemp;
}

int FlowState::fanRpm()const
{
	return m_fanRpm;
}

int FlowState::pumpRpm()const
{
	return m_pumpRpm;
}

time_t FlowState::timestamp()const
{
	return m_timestamp;
}
