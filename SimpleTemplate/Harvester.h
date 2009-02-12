#ifndef HARVESTER_H__
#define HARVESTER_H__

#include "WPILib.h"
#include "Target.h"
#include "LimitSwitch.h"
#include "Toggle.h"

// #include "Robot.cpp"

#define HARV_STATE_NOT_FULL 0
#define HARV_STATE_FULL 1

#define HARV_AUTO_MODE_COLLECT 0
#define HARV_AUTO_MODE_LOAD 1
#define HARV_AUTO_MODE_EJECT 2
#define HARV_AUTO_MODE_IDLE 3

class RobotHarvester
{
public:
	RobotHarvester(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel);
	~RobotHarvester();
	
	void SetHarvesterControls(Joystick *Stick, UINT32 Button);
	void SetEjecterControls(Joystick *Stick, UINT32 Button);
	void Init(void);
	void Process(bool LoadElevator);
	UINT32 GetState(void);
	void SetState(UINT32 State);
	UINT32 GetAutoMode(void);
	void SetAutoMode(UINT32);
	
private:
	////////////////////////////////////////////////////////
	// HARDWARE
	Jaguar *HarvesterMotor;
	AnalogChannel *HarvesterMotorCurrent;
	Joystick *HarvesterStick;
	UINT32 HarvesterButton;
	Joystick *EjecterStick;
	UINT32 EjecterButton;
	Toggle *HarvesterToggle;
	Toggle *EjecterToggle;
	
	////////////////////////////////////////////////////////
	// STATE
	UINT32 State;
	UINT32 AutoMode;
};

#endif
