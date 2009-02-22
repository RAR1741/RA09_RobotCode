#ifndef JOYSTICK_PID_SOURCE_H__
#define JOYSTICK_PID_SOURCE_H__

/**************************************************************************
 * JoystickPIDSource.h
 * 
 * This code attempts to use a joystick as a PID source, which it is already
 * capable of doing. However, this will also do the usual limit switching.
 * 
 * This code may not be of much utility in the competition, but is helpful
 * in figuring out the PID mechanism.
 **************************************************************************/

#include "LimitedPIDSource.h"

#include "Joystick.h"

class JoystickPIDSource : LimitedPIDSource 
{
public:
	JoystickPIDSource();
	
	void SetJoystick(Joystick *stick, Joystick::AxisType axis);
	
protected:
	virtual void Poll(void);
private:
	Joystick *m_Stick;	// Stick to use as source
	Joystick::AxisType m_Axis;	// Axis to use from Joystick
};

#endif
