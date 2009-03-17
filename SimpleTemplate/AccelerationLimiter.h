#ifndef ACCELERATION_LIMITER_H__
#define ACCELERATION_LIMITER_H__

/*********************************************************************************
 * AccelerationLimiter
 * 
 * A class that controls and limits the change in a variable in between calls.
 * 
 * If you are of a calculus bent, it works like a governer on a diesel engine on 
 * an 18-wheeler. The truck gets to the same destination, but possibly slower than
 * it would without the governer. In the same way, through a derivative
 * transformation, this acceleration governer limits the change in velocity, rather
 * than position, per unit time. The target velocity will still be reached, but
 * possibly more slowly.
 *********************************************************************************/
class AccelerationLimiter
{
public:
	AccelerationLimiter(float unitTime, float maxChange);
	~AccelerationLimiter();
	
	// MUST CALL ONLY ONCE PER LOOP PER USAGE
	void StartNewIteration(float currentInput);
	
	void NotifyWhileIgnored(float currentInput);
	
	float GetCorrectedInput(void);
	
private:
	
	float m_unitTime;	// Unit time, like 1/200th of a second. Should
						// be the period that StartNewIteration is called.
	float m_unitChange;	// The maximum change in the input allowed per unit
						// time.
	float m_last;		// Old value used for state.
};

#endif

