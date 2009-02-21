#include "Elevator.h"
#include "DriverStationLCD.h"
#include "Mode.h"


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
//	ElevatorMotorCurrent = new AnalogChannel(CurrentSlot, CurrentChannel);
	ElevatorButton = 0;
	State = 0;
	AutoMode = 0;
	ElevatorMotor->Set(0.0);
	ElevatorEncoder = new Encoder(4,11,4,12,false);
	ElevatorEncoder->Start();
	ElevatorMotorCurrent = new CurrentSensor();
	ElevatorMotorCurrent->Init(2,1,2.5,CurrentSensor::m_20Amp);
	isFull = false;
	isJammed = false;
	HomeItFlag = false;
	CycleFlag = false;
	UntripFlag = false;
	HomeSwitch = new LimitSwitch(6,6);
	CurrentElevatorEncoderValue = 0;
	LastElevatorEncoderValue = 0;
	//IsClearingJam = false;
}

void RobotElevator::Process()
{
	DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
	// Common LCD update
	// AutoMode = MODE_MANUAL;
	if(AutoMode == MODE_MANUAL){// Put Manual/Auto if condition here. 
			// Manual Mode
		if (ElevatorStick != NULL && theToggle != NULL){
					theToggle->UpdateState();
					
					if (ElevatorStick->GetRawButton(1) && !isJammed)
						ElevatorMotor->Set(.5);
					else if(!isJammed){
						ElevatorMotor->Set(0);
					}
					if(isJammed)
						ClearJam(-.5);
					else
						DetectJams();
		}
	}
		else{
			// Auto mode code should be here.
			if(ElevatorStick->GetRawButton(6))
				ElevatorEncoder->Reset();
			if(ElevatorStick->GetRawButton(7))
				HomeItFlag=true;
			if(HomeItFlag && !CycleFlag && !isJammed)
				HomeIt(); // Home at 1/4 speed
			if(ElevatorStick->GetRawButton(1) && !HomeItFlag && !CycleFlag && !isJammed){
				CycleFlag=true;
				UntripFlag=true;
			}
			if(CycleFlag){ // if we are cycling
					Cycle(.5);
			}
			//DetectJams();
			if(isJammed)
				ClearJam(-.5);
			else
				DetectJams();
				
		}
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 1, "IJ:%1d",isJammed);
		//DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 5, "MC:%2.1f",ElevatorMotorCurrent->GetCurrent());
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 14, "EN:%2d",(CurrentElevatorEncoderValue-LastElevatorEncoderValue));
		dsLCD->UpdateLCD();
						//dsLCD->UpdateLCD();
		LastElevatorEncoderValue = CurrentElevatorEncoderValue;
		CurrentElevatorEncoderValue = ElevatorEncoder->Get();
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
		else{
			if(motorSpeed>0){
			// Otherwise...
			
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
}
void RobotElevator::DetectJams()
{
	bool controlFlag;
	if(AutoMode==MODE_MANUAL){
		if(ElevatorStick->GetRawButton(1))
			controlFlag = true;
		else controlFlag = false;
	}
	else{
		if(CycleFlag)
			controlFlag = true;
		else controlFlag = false;
	}
	if(controlFlag){
	if(ElevatorEncoder!=NULL && ElevatorMotorCurrent!=NULL){ // Do we have valid ptrs to use?
		if(ElevatorMotorCurrent->GetCurrent() >= 14){ // Is the motor voltage on?
// Colin: Remind me tonight to change the sign. I thought of an easy way to make it
// output positive values with going forward. We may need this on others as well.  Hugh
			//if((CurrentElevatorEncoderValue - LastElevatorEncoderValue)<=7){
							if(ElevatorMotor->Get()>0){
					
								// If all the above conditions have been meant then...
								ElevatorMotor->Set(0.0);
								//Cycle(-.5);
								isJammed = true;
								HomeItFlag = false;
								CycleFlag = false; // stop cycling
								UntripFlag = true;
							}
				//	dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "ERR: Evtr Jammed.");
				//	dsLCD->UpdateLCD();
	//		}
		}
	}
	}
	else isJammed = false;	
}

	void RobotElevator::ClearJam(float motorSpeed)
	{

		// Cycle Bacckwords code.
		if(UntripFlag){// Do we need to untrip the switch?
			// Then set motor and wait until it's untripped.
			ElevatorMotor->Set(motorSpeed);
			if(HomeSwitch->IsTripped())
				UntripFlag=false;
		}
		else{
		if(ElevatorEncoder->Get()>450)
		ElevatorMotor->Set(motorSpeed);
		if(ElevatorEncoder->Get()>200)
		ElevatorMotor->Set(motorSpeed/1.5);
		else
		ElevatorMotor->Set(motorSpeed/2.0);
		if(!HomeSwitch->IsTripped()) {// If this goes, then the limit switch is tripped.
			isJammed=false;// Set false so not called again.
			ElevatorMotor->Set(0);
			ElevatorEncoder->Reset();// Encoder must reset for another possible cycle.
		}
	}
}
