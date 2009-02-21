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
	RunStopToggle = new Toggle(ElevatorStick, 11);
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
	RunStop = false;
	BusyFlag = false;
	LoadHarvesterLoad = false;
	LauncherStatus = false;
	HomeSwitch = new LimitSwitch(6,6);
	CurrentElevatorEncoderValue = 0;
	LastElevatorEncoderValue = 0;
	//IsClearingJam = false;
}

void RobotElevator::Process(bool LauncherStatus)
{
	this->LauncherStatus = LauncherStatus;
    DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
	// Common LCD update
	// AutoMode = MODE_MANUAL;
	if(AutoMode == MODE_MANUAL){// Put Manual/Auto if condition here. 
			// Manual Mode
		if (ElevatorStick != NULL){
					
					
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
		else if(AutoMode == MODE_SEMI_AUTO){
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
				
		}/*
		else{
			switch(State){
				case 0:
					if(RunStop)
						State = 1;
					else{
						State = 0;
						ElevatorMotor->Set(0);
					}
					break;
					
				case 1:
					HomeItFlag = true;
					State = 2;
					break;
				case 2:
					DetectJams();
					if(HomeItFlag)
						HomeIt();
					if(isJammed)
						State = 10;
					else if(!BusyFlag)
						State = 3;
					else
						State = 2;
					break;
				case 3:
					DetectJams();
					if(isJammed)
						State = 11;
					else if(!BusyFlag)
						State = 13;
					else
						State = 3;
					break;
				case 4:
					LoadHarvesterLoad = true;
					State = 7;
					break;
					
				case 5:
					if(ElevatorStick->GetRawButton(1)){
						CycleFlag = true; // Signal to cycle the elevator
						UntripFlag = true; // must "dehome it" first
						State = 6;
					}
					else
						State = 5;
					break;
					
				case 6:
					LoadHarvesterLoad = false;
					State = 8;
					break;
				case 7:
					if(this->LauncherStatus)
						State = 5;
					else State = 7;
					break;
					
				case 8:
					if(CycleFlag){
						Cycle(.5);
						State = 8;
					}
					else
						State = 3;
				case 9:
					if(isJammed){
						ClearJam(-.5);
						State = 9;
					}
					else if(!isJammed){
						State = 1;
					}
					break;
				case 10:
					State = 9;
					break;
				case 11:
					if(isJammed){
						ClearJam(-.5);
						State = 11;
					}
					else
						State = 12;
					break;
				case 12:
					State = 3;
					break;
				case 13:
					isFull = false;
					State = 4;
					break;
			}
		}*/
	
		/*
		//DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 5, "MC:%2.1f",ElevatorMotorCurrent->GetCurrent());
		dsLCD->Printf(DriverStationLCD::kUser_Line3, 14, "EN:%4d",ElevatorEncoder->Get());
		dsLCD->UpdateLCD();*/
		dsLCD->UpdateLCD();
		LastElevatorEncoderValue = CurrentElevatorEncoderValue;
		CurrentElevatorEncoderValue = ElevatorEncoder->Get();
		RunStopToggle->UpdateState();
}
bool RobotElevator::IsFull()
{
	return isFull;
}


void RobotElevator::HomeIt()
{
		ElevatorMotor->Set(.25);
		BusyFlag = true;
		if(!HomeSwitch->IsTripped()){
			ElevatorMotor->Set(0);
			HomeItFlag=false;
			BusyFlag = false;
			ElevatorEncoder->Reset();
		}
}

void RobotElevator::Cycle(float motorSpeed)
{
		BusyFlag = true;
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
				BusyFlag = false;
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
		BusyFlag = true;
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
			BusyFlag = false;
			ElevatorMotor->Set(0);
			ElevatorEncoder->Reset();// Encoder must reset for another possible cycle.
		}
	}
}

bool RobotElevator::GetHarvesterLoad()
{
	return LoadHarvesterLoad;
}
