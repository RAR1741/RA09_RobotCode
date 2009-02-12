#ifndef TOGGLE_H__
#define TOGGLE_H__

#include "WPILib.h"
#include "Target.h"
#include "LimitSwitch.h"

// #include "Robot.cpp"

class Toggle
{
public:
	Toggle(Joystick *Stick, UINT32 Button);
	~Toggle();
	
	void UpdateState(void);
	bool GetOutput(void);
	void Reset(void);
	
private:
	////////////////////////////////////////////////////////
	// HARDWARE
	Joystick *ToggleStick;
	UINT32 StickButton;
	
	////////////////////////////////////////////////////////
	// STATE
	UINT32 State;
	bool Output;
};

#endif
