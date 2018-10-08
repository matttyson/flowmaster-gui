#ifndef LOGGER_HPP
#define LOGGER_HPP

#include "flowstate.hpp"
#include <QList>


class Logger
{
public:
	Logger();

	void addItem(const FlowState &item);

	// Flush the list out to disk
	void flush();

private:
	QList<FlowState> m_FlowStates;
};

#endif // LOGGER_HPP
