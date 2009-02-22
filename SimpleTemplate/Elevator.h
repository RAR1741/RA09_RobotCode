#ifndef ELEVATOR_H__
#define ELEVATOR_H__

#include "WPILib.h"
#include "toggle.h"
#include "launcher.h"
#include "CurrentSensor.h"
#include "Harvester.h"
// #include "Robot.cpp"

#define ELEVATOR_STATE_FULL 0
#define ELEVATOR_STATE_EMPTY 1


class RobotElevator
{
public:
	RobotElevator();
	~RobotElevator();
	
	void SetElevatorControls(Joystick *Stick, UINT32 Button);
	void Init(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel, RobotHarvester * pHarvester);
	void Process(bool LauncherStatus, bool RunStop);
	UINT32 GetState(void);
	void SetState(UINT32 State);
	UINT32 GetAutoMode(void);
	void SetAutoMode(UINT32);
	bool IsFull(void);
	void HomeIt();
	void Cycle(float MotorSpeed);
	void DetectJams();
	void ClearJam(float MotorSpeed);
	bool GetHarvesterLoad();
private:
	////////////////////////////////////////////////////////
	// HARDWARE
	Jaguar *ElevatorMotor;
	Encoder * ElevatorEncoder;
	// Jaguar * LaunchMotor;
	CurrentSensor * ElevatorMotorCurrent;
	Joystick *ElevatorStick;
	RobotHarvester * theHarvester;
	// Toggle * RunStopToggle;
	// Encoder ElevatorEncoder;
	// Launcher * launcher;
	UINT32 ElevatorButton;
	LimitSwitch * HomeSwitch;
	
	////////////////////////////////////////////////////////
	// STATE
	UINT32 State;
	UINT32 AutoMode;
	float launchDistance;
	bool ArmDisarmed;
	// bool RunStop;
	bool ElevatorAutoMode;
	bool isFull;
	bool isJammed;
	bool BusyFlag;
	bool LoadHarvesterLoad;
	// flag for homing function
	bool HomeItFlag;
	// flag for regular cycle
	bool CycleFlag;
	// flag for untripping switch before cycling
	bool UntripFlag;
	bool LauncherStatus;
	int CurrentElevatorEncoderValue;
	int LastElevatorEncoderValue;
};

#endif
