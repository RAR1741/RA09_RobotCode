#include "Elevator.h"

RobotElevator::RobotElevator(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel)
{
	ElevatorMotor = new Jaguar(MotorSlot, MotorChannel);
	ElevatorMotorCurrent = new AnalogChannel(CurrentSlot, CurrentChannel);
	ElevatorButton = 0;
	State = 0;
	AutoMode = 0;
}

RobotElevator::~RobotElevator()
{
	ElevatorMotor = NULL;
	ElevatorMotorCurrent = NULL;
	ElevatorStick = NULL;
}

UINT32 RobotElevator::GetState(void)
{
	return State;
}

void RobotElevator::SetState(UINT32 NewState)
{
	State = NewState;
}

UINT32 RobotElevator::GetAutoMode(void)
{
	return AutoMode;
}

void RobotElevator::SetAutoMode(UINT32 NewAutoMode)
{
	AutoMode = NewAutoMode;
}

void RobotElevator::SetElevatorControls(Joystick *Stick, UINT32 Button) 
{ 
	ElevatorStick = Stick;
	ElevatorButton = Button;
}

void RobotElevator::Init(void)
{
	State = 0;
	AutoMode = 0;
	ElevatorMotor->Set(0.0);
}

void RobotElevator::Process(void)
{
	if(1){// Put Manual/Auto if condition here. 
		// Manual Mode
		if (ElevatorStick != NULL)
		{
			if (ElevatorStick->GetRawButton(ElevatorButton))
				ElevatorMotor->Set(.5);
			else
				ElevatorMotor->Set(0.0);
		}
	}
	else{
		// Auto mode code should be here.
	}
}

