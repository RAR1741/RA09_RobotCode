#include "AccelerationLimiter.h"

#include <cmath> // For handy mathematics functions :)

using ::fabs;

AccelerationLimiter::AccelerationLimiter(float unitTime, float maxChange)
{
	this->m_unitTime = unitTime;
	this->m_maxChange = maxChange;
	this->m_old = 0.0;	// Motors and stuff are typically initialized to
						// zero.
}

AccelerationLimiter::~AccelerationLimiter()
{
	// Doesn't need to do anything.
}

void AccelerationLimiter::StartNewIteration(float currentInput)
{
	// Calculate new accepted input.
	float gross_change = currentInput - this->m_old;
	
	float normalized_acceleration = gross_change / this->m_unitTime;
	
	
}