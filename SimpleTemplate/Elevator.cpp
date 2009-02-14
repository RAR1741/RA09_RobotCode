#include "Elevator.h"
#include "Launcher.h"

RobotElevator::RobotElevator()
{

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
	theToggle = new Toggle(ElevatorStick, Button);
}

void RobotElevator::Init(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel, Encoder * elevEncoder)
{
	State = 0;
	AutoMode = 0;
	ElevatorMotor = new Jaguar(MotorSlot, MotorChannel);
	// LaunchMotor = new Jaguar(LaunchMotorSlot, LaunchMotorChannel);
	ElevatorMotorCurrent = new AnalogChannel(CurrentSlot, CurrentChannel);
	ElevatorButton = 0;
	State = 0;
	AutoMode = 0;
	ElevatorMotor->Set(0.0);
	ElevatorEncoder = elevEncoder;
	// launcher = theLauncher;
}

void RobotElevator::Process()
{
	
	if(1){// Put Manual/Auto if condition here. 
			// Manual Mode
		if (ElevatorStick != NULL && theToggle != NULL){
				theToggle->UpdateState();
				if (theToggle->GetOutput()){
					ElevatorMotor->Set(.5);
					// This scales it to 0 - 1, the double negatives ARE correct.
				// 	LaunchMotor->Set(-((-ElevatorStick->GetZ()+1.0)/2.0));
					// launcher->SetRun(true); // Allow update() to run.
				}
				else{
					ElevatorMotor->Set(0.0);
					//LaunchMotor->Set(0.0);
					// launcher->SetRun(false); // We want it to stop running
				}
				
		}
		else{
			// Auto mode code should be here.
		}
	}
}

bool RobotElevator::IsFull()
{
	return isFull;
}


