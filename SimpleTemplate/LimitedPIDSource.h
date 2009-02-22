#ifndef LIMITED_PID_SOURCE_H__
#define LIMITED_PID_SOURCE_H__

/**************************************************************************
 * LimitedPIDSource.h
 * 
 * This file is an attempt to standardize the use of a PID loop as the source
 * of input to a rotational hardware apparatus, such as a turret on Squeeky.
 * 
 * The forums at ChiefDelphi suggest bringing the logic for limiting into the
 * source of the PID loop, which is pretty stupid, so that the PID loop
 * does not fight conventional code.
 * 
 * 
 **************************************************************************/
#include "PIDSource.h"
#include "LimitSwitch.h"

class LimitedPIDSource : PIDSource
{
public:
	LimitedPIDSource();
	
	virtual void InitNew(int cwLimitSlot, int cwLimitChannel, int ccwLimitSlot, int ccwLimitChannel, bool cwPos = false);
	virtual void InitLimitSwitches(LimitSwitch *cw, LimitSwitch *ccw, bool cwPos = false);
	
	virtual double PIDGet();
	
	// Sets whether the clockwise limit switch limits in the positive (true) or 
	// negative (false) direction. False by default. 
	virtual void SetClockwiseIsPositive(bool flag);
protected:
	
	// Use this in future derivatives to set the source,
	// PIDGet is simply going to return it plus 0 if limited
	virtual void SetPIDGet(double foo);
	
	// Overide this to set the PIDGet retval when it is accessed. Automagically called
	// in PIDGet.
	virtual void Poll(void);
private:
	
	// State
	bool m_ClockwiseIsPositive;
	double m_PIDGet;
		
	// Hardware.
	
	LimitSwitch *Clockwise_Limit, *CounterClockwise_Limit;
};

#endif
