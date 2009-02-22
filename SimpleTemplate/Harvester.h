#ifndef HARVESTER_H__
#define HARVESTER_H__

#include "WPILib.h"
#include "Toggle.h"
#include "Mode.h"
#include "GateController.h"
 
// #include "Robot.cpp"

#define HARV_STATE_NOT_FULL 0
#define HARV_STATE_FULL 1

#define COLLECT_MOTOR_SPEED .5
#define EJECT_MOTOR_SPEED -.5
#define MOTOR_STOP 0.0

#define HARV_AUTO_MODE_COLLECT 0
#define HARV_AUTO_MODE_LOAD 1
#define HARV_AUTO_MODE_EJECT 2
#define HARV_AUTO_MODE_IDLE 3

class RobotHarvester
{
public:
	RobotHarvester(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel);
	~RobotHarvester();
	
	void SetCollectEjectControls(Joystick *Stick, UINT32 Button);
	void SetRunStopControls(Joystick *Stick, UINT32 Button);
	void SetGateControls(GateController *Gate);
	void Init(void);
	void Process(bool LoadElevator, bool RunStop);
	UINT32 GetState(void);
	void SetState(UINT32 State);
	UINT32 GetAutoMode(void);
	void SetAutoMode(UINT32);
	float GetHarvesterMotorVoltage(void);
	
private:
	void ProcessManual(void);
	void ProcessSemiAuto(bool LoadElevator);
	void ProcessAuto(bool LoadElevator, bool RunStop);
	
	////////////////////////////////////////////////////////
	// HARDWARE
	Jaguar *HarvesterMotor;
	AnalogChannel *HarvesterMotorCurrent;
	Joystick *RunStopStick;
	UINT32 RunStopButton;
	Joystick *CollectEjectStick;
	UINT32 CollectEjectButton;
	Toggle *RunStopToggle;
	Toggle *CollectEjectToggle;
	GateController *TheGate;
	
	////////////////////////////////////////////////////////
	// STATE
	UINT32 State;
	UINT32 AutoMode;
	bool HarvesterFull;
	float HarvesterMotorVoltage;
	UINT32 HarvesterMode;
};

#endif
