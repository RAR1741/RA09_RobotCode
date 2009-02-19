#ifndef ELEVATOR_H__
#define ELEVATOR_H__

#include "WPILib.h"
#include "toggle.h"
#include "launcher.h"
// #include "Robot.cpp"

#define ELEVATOR_STATE_FULL 0
#define ELEVATOR_STATE_EMPTY 1


class RobotElevator
{
public:
	RobotElevator();
	~RobotElevator();
	
	void SetElevatorControls(Joystick *Stick, UINT32 Button);
	void Init(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel);
	void Process();
	UINT32 GetState(void);
	void SetState(UINT32 State);
	UINT32 GetAutoMode(void);
	void SetAutoMode(UINT32);
	bool IsFull(void);
	void HomeIt();
	void Cycle(float MotorSpeed);
	void DetectJams();
	void ClearJam(float MotorSpeed);
private:
	////////////////////////////////////////////////////////
	// HARDWARE
	Jaguar *ElevatorMotor;
	Encoder * ElevatorEncoder;
	// Jaguar * LaunchMotor;
	AnalogChannel * ElevatorMotorCurrent;
	Joystick *ElevatorStick;
	Toggle * theToggle;
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
	bool RunStop;
	bool ElevatorAutoMode;
	bool isFull;
	bool isJammed;
	// flag for homing function
	bool HomeItFlag;
	// flag for regular cycle
	bool CycleFlag;
	// flag for untripping switch before cycling
	bool UntripFlag;
	bool CurrentElevatorEncoderValue;
	bool LastElevatorEncoderValue;
};

#endif
