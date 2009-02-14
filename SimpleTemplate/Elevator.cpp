#include "Elevator.h"
#include "Launcher.h"

#define ANTI_JAM 0 // I don't want the code to be used yet.
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

void RobotElevator::Init(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel)
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
	ElevatorEncoder = new Encoder(4,11,4,12,false);
	ElevatorMotorCurrent = new AnalogChannel(2,1);
	LastElevatorEncoderValue = CurrentElevatorEncoderValue = 0;
	isFull = isJammed = false;
	// launcher = theLauncher;
}

void RobotElevator::Process()
{
	
	if(1){// Put Manual/Auto if condition here. 
			// Manual Mode
		if (ElevatorStick != NULL && theToggle != NULL){
					theToggle->UpdateState();
					if (theToggle->GetOutput() && !isJammed){
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
	// Set encoder values.	
	LastElevatorEncoderValue = CurrentElevatorEncoderValue;
	CurrentElevatorEncoderValue = ElevatorEncoder->Get();
	
	// Anti Jamming Code.
#if ANTI_JAM
	if(ElevatorEncoder!=NULL && ElevatorMotorCurrent!=NULL){ // Do we have valid ptrs to use?
		if(ElevatorMotorCurrent->GetVoltage()>=4.2 &&
			ElevatorMotorCurrent->GetVoltage() <=5.2){ // Is the motor voltage on?
			if(!isJammed){ // Has a jam not recently been detected?
				if((CurrentElevatorEncoderValue - LastElevatorEncoderValue)<=250){ 
					// Is the encoder getting a smaller
					// than normal rate for the motor being on?
					
					// If all the above conditions have been meant then...
					ElevatorMotor->Set(0.0);
					isJammed = true;
				}
			}
		}
	}
	else isJammed = false;
#endif
}
bool RobotElevator::IsFull()
{
	return isFull;
}


