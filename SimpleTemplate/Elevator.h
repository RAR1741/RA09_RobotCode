#ifndef ELEVATOR_H__
#define ELEVATOR_H__

#include "WPILib.h"

// #include "Robot.cpp"

#define ELEVATOR_STATE_FULL 0
#define ELEVATOR_STATE_EMPTY 1


class RobotElevator
{
public:
	RobotElevator(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel);
	~RobotElevator();
	
	void SetElevatorControls(Joystick *Stick, UINT32 Button);
	void Init(void);
	void Process(void);
	UINT32 GetState(void);
	void SetState(UINT32 State);
	UINT32 GetAutoMode(void);
	void SetAutoMode(UINT32);
	
private:
	////////////////////////////////////////////////////////
	// HARDWARE
	Jaguar *ElevatorMotor;
	AnalogChannel *ElevatorMotorCurrent;
	Joystick *ElevatorStick;
	UINT32 ElevatorButton;
	
	////////////////////////////////////////////////////////
	// STATE
	UINT32 State;
	UINT32 AutoMode;
};

#endif
