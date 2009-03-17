#include "AccelerationLimiter.h"

#include <cmath> // For handy mathematics functions :)

using ::fabs;

AccelerationLimiter::AccelerationLimiter(float unitTime, float maxChange)
{
	this->m_unitTime = unitTime;
	this->m_unitChange = maxChange * unitTime;
	this->m_last = 0.0;	// Motors and stuff are typically initialized to
						// zero.
}

AccelerationLimiter::~AccelerationLimiter()
{
	// Doesn't need to do anything.
}

void AccelerationLimiter::StartNewIteration(float currentInput)
{
	// Calculate new accepted input.
	float delta_v = currentInput - m_last;
	
	float corr_accel = 0.0;
	
	if (fabs(delta_v) > m_unitChange) {
		corr_accel = (delta_v < 0) ? - m_unitChange : m_unitChange;
	} else {
		corr_accel = delta_v;
	}
	
	m_last += corr_accel;
	
	
}

float AccelerationLimiter::GetCorrectedInput()
{
	return m_last;
}


void AccelerationLimiter::NotifyWhileIgnored(float currentInput)
{
	m_last = currentInput;
}
