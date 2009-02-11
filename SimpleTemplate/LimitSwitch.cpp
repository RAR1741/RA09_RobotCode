#include "LimitSwitch.h"

LimitSwitch::LimitSwitch(UINT32 channel) : DigitalInput(channel)
{
	//this->DigitalInput(channel);
	
}

LimitSwitch::LimitSwitch(UINT32 slot, UINT32 channel) : DigitalInput(slot, channel)
{
	//this->DigitalInput(slot, channel)
}

LimitSwitch::~LimitSwitch(void)
{
	
}

bool LimitSwitch::IsTripped(void)
{
	return (this->Get() != 0);
}

