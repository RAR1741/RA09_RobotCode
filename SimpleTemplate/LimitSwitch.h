#ifndef LIMIT_SWITCH_H__
#define LIMIT_SWITCH_H__

#include "WPILib.h"

class LimitSwitch : DigitalInput
{
public:
	
	// The "explicit" prevents a nasty ambiguity involving
	// type-casting to integers...
	explicit LimitSwitch(UINT32 channel) : DigitalInput(channel) {}
	LimitSwitch(UINT32 slot, UINT32 channel) : DigitalInput(slot, channel) {}
	~LimitSwitch();
	
	bool IsTripped(void) { return (this->Get() != 0); }
	
	//TODO Add code to limit motors here
	void LimitPositive(float &motor) { if (this->IsTripped() && motor > 0) motor = 0; }	
	void LimitNegative(float &motor) { if (this->IsTripped() && motor < 0) motor = 0; }
};

#endif
