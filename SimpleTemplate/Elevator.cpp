#include "Elevator.h"
#include "DriverStationLCD.h"


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
	ElevatorEncoder->Start();
	ElevatorMotorCurrent = new AnalogChannel(2,1);
	LastElevatorEncoderValue = CurrentElevatorEncoderValue = 0;
	isFull = false;
	isJammed = false;
	HomeItFlag = false;
	CycleFlag = false;
	UntripFlag = false;
	HomeSwitch = new LimitSwitch(6,6);
	// launcher = theLauncher;
}

void RobotElevator::Process()
{
	
	if(false){// Put Manual/Auto if condition here. 
			// Manual Mode
		if (ElevatorStick != NULL && theToggle != NULL){
					theToggle->UpdateState();
					
					// If the trigger has been pushed, fire semi automatic
					if (ElevatorStick->GetRawButton(1) && !isJammed && !HomeItFlag){
					
						//if(!CycleFlag){//We don't want to do this if Cycle flag is already true
						ElevatorMotor->Set(.5);
						//CycleFlag=true;
						//tell HomeIt() to cycle the elevator
						//}
					}
					else
						ElevatorMotor->Set(0);
					

					DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
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
			if(HomeItFlag)
				HomeIt(); // Home at 1/4 speed
			if(ElevatorStick->GetRawButton(1) && !HomeItFlag && !CycleFlag){
				CycleFlag=true;
				UntripFlag=true;
			}
			if(CycleFlag){ // if we are cycling
					Cycle(.5);
			}
			else if(!HomeItFlag){
				//ElevatorMotor->Set(0.0);
				//LaunchMotor->Set(0.0);
				// launcher->SetRun(false); // We want it to stop running
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


void RobotElevator::HomeIt()
{
		ElevatorMotor->Set(.25);// Otherwise, just use motorspeed.
		
		if(!HomeSwitch->IsTripped()){
			ElevatorMotor->Set(0);
			HomeItFlag=false;
			ElevatorEncoder->Reset();
			//isFirstTime=true; 
			// This will reset homeit to reset the encoder again the next cycle.
		}
		
}

void RobotElevator::Cycle(float motorSpeed)
{
	if(UntripFlag){// Do we need to untrip the switch?
		ElevatorMotor->Set(motorSpeed);
		if(HomeSwitch->IsTripped())
			UntripFlag=false;
	}
	else{
		if(ElevatorEncoder->Get()<800)
			ElevatorMotor->Set(motorSpeed);
		else if(ElevatorEncoder->Get()<950)
			ElevatorMotor->Set(motorSpeed/1.5);
		else
			ElevatorMotor->Set(motorSpeed/2.0);
		if(!HomeSwitch->IsTripped()){
			CycleFlag=false;
			ElevatorMotor->Set(0);
			ElevatorEncoder->Reset();
		}
	}
}
