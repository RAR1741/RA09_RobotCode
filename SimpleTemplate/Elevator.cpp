#include "Elevator.h"
#include "DriverStationLCD.h"


#define ANTI_JAM 0

// IMPORTANT COMMENT: the logic for LimitSwitch.IsTripped(), as of this comment,
// is inverted. The ! operators in front of some of the if statements are correct,
// do not modify unless Patrick has fixed his class.
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
	ElevatorMotorCurrent = new AnalogChannel(CurrentSlot, CurrentChannel);
	ElevatorButton = 0;
	State = 0;
	AutoMode = 0;
	ElevatorMotor->Set(0.0);
	ElevatorEncoder = new Encoder(4,11,4,12,false);
	ElevatorEncoder->Start();
	ElevatorMotorCurrent = new AnalogChannel(2,1);
	isFull = false;
	isJammed = false;
	HomeItFlag = false;
	CycleFlag = false;
	UntripFlag = false;
	HomeSwitch = new LimitSwitch(6,6);
}

void RobotElevator::Process()
{
	DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
	// Common LCD update
	if(true){// Put Manual/Auto if condition here. 
			// Manual Mode
		if (ElevatorStick != NULL && theToggle != NULL){
					theToggle->UpdateState();
					
					if (ElevatorStick->GetRawButton(1) && !isJammed)
						ElevatorMotor->Set(.5);
					else
						ElevatorMotor->Set(0);
					
					if(ElevatorEncoder==NULL){
						dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "ERR");
					}
					else
					dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "Elevator Encoder:%4d",ElevatorEncoder->Get());
					dsLCD->UpdateLCD();
		}
	}
		else{
			// Auto mode code should be here.
			if(ElevatorStick->GetRawButton(6))
				ElevatorEncoder->Reset();
			if(ElevatorStick->GetRawButton(7))
				HomeItFlag=true;
			if(HomeItFlag && !CycleFlag)
				HomeIt(); // Home at 1/4 speed
			if(ElevatorStick->GetRawButton(1) && !HomeItFlag && !CycleFlag){
				CycleFlag=true;
				UntripFlag=true;
			}
			if(CycleFlag){ // if we are cycling
					Cycle(.5);
			}
		}	
	// Anti Jamming Code.
#if ANTI_JAM
	if(ElevatorEncoder!=NULL && ElevatorMotorCurrent!=NULL){ // Do we have valid ptrs to use?
		if(ElevatorMotorCurrent->GetVoltage()>=4.8 &&
			ElevatorMotorCurrent->GetVoltage() <=5.2){ // Is the motor voltage on?
			if(!isJammed){ // Has a jam not recently been detected?
				if((CurrentElevatorEncoderValue - LastElevatorEncoderValue)<=50){ 
					// Is the encoder getting a smaller
					// than normal rate for the motor being on?
					
					// If all the above conditions have been meant then...
					ElevatorMotor->Set(0.0);
					isJammed = true;
					dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "ERR: Evtr Jammed.");
					dsLCD->UpdateLCD();
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


void RobotElevator::HomeIt()
{
		ElevatorMotor->Set(.25);
		
		if(!HomeSwitch->IsTripped()){
			ElevatorMotor->Set(0);
			HomeItFlag=false;
			ElevatorEncoder->Reset();
		}
}

void RobotElevator::Cycle(float motorSpeed)
{
	if(UntripFlag){// Do we need to untrip the switch?
		// Then set motor and wait until it's untripped.
		ElevatorMotor->Set(motorSpeed);
		if(HomeSwitch->IsTripped())
			UntripFlag=false;
	}
	else{// Otherwise...
		
		// Slowdown code, uses encoder to determine speed...
		// The encoder reads about 1130 to 1170 counts when a cycle completes.
		// So...
		
		if(ElevatorEncoder->Get()<800)
			ElevatorMotor->Set(motorSpeed);
			// If we still have less than  8/11ths of the way to go
			// set full motor speed
		else if(ElevatorEncoder->Get()<950)
			ElevatorMotor->Set(motorSpeed/1.5);
			// when we read greater than 800, but less than 950, we're getting close
			// so cut speed a little.
		else
			ElevatorMotor->Set(motorSpeed/2.0);
			// Otherwise, we're getting super close, so cut in half.
		
		if(!HomeSwitch->IsTripped()){// If this goes, then the limit switch is tripped.
			CycleFlag=false;// Set false so not called again.
			ElevatorMotor->Set(0);
			ElevatorEncoder->Reset();// Encoder must reset for another possible cycle.
		}
	}
}
