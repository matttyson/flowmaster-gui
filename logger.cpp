#include "logger.hpp"

Logger::Logger()
{
}

void Logger::addItem(const FlowState &item)
{
	m_FlowStates.append(item);
}

void Logger::flush()
{
	/*
	foreach(const FlowState &item, m_FlowStates){

	}
	*/
}
