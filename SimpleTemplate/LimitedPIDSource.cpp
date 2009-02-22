
#include "LimitedPIDSource.h"

LimitedPIDSource::LimitedPIDSource()
{
	Clockwise_Limit = NULL;
	CounterClockwise_Limit = NULL;
	
	m_ClockwiseIsPositive = false;	// Default for a worm-gear setup
}

void LimitedPIDSource::Init(int cwLimitSlot, int cwLimitChannel,
							int ccwLimitSlot, int ccwLimitChannel,
							bool cwPos)
{
	Clockwise_Limit = new LimitSwitch(cwLimitSlot, cwLimitChannel);
	CounterClockwise_Limit = new LimitSwitch(ccwLimitSlot, ccwLimitChannel);
	m_ClockwiseIsPositive = cwPos;
}

void LimitedPIDSource::SetClockwiseIsPositive(bool flag)
{
	m_ClockwiseIsPositive = flag;
}

void LimitedPIDSource::SetPIDGet(double foo)
{
	m_PIDGet = foo;
}

double LimitedPIDSource::PIDGet()
{
	double retval = m_PIDGet;
	
	if (retval < 0) {
		if (m_ClockwiseIsPositive) {
			CounterClockwise_Limit->LimitNegative(retval);
		} else {
			Clockwise_Limit->LimitNegative(retval);
		}
	} else {
		if (m_ClockwiseIsPositive) {
			Clockwise_Limit->LimitPositive(retval);
		} else {
			CounterClockwise_Limit->LimitPositive(retval);
		}
	}
	
	return retval;
}

void LimitedPIDSource::Poll()
{
	// NOP
}
