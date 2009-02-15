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
	HomeSwitch = new LimitSwitch(6,6);
	// launcher = theLauncher;
}

void RobotElevator::Process()
{
	
	if(1){// Put Manual/Auto if condition here. 
			// Manual Mode
		if (ElevatorStick != NULL && theToggle != NULL){
					theToggle->UpdateState();
					
					// If the trigger has been pushed, fire semi automatic
					if (ElevatorStick->GetRawButton(1) && !isJammed){
					
						if(!CycleFlag){//We don't want to do this if Cycle flag is already true
						ElevatorMotor->Set(.5);
						CycleFlag=true;
						//tell HomeIt() to cycle the elevator
						}
					}
					
					if(CycleFlag){ // if we are cycling
							HomeIt(.5, &CycleFlag, true);
							// call Homeit, start running motor at half speed,
							// use CycleFlag as call flag, and the last parameter
							// tells HomeIt that we want to slow down as it approached
							// the limit switch.
					}
					else{
						ElevatorMotor->Set(0.0);
						//LaunchMotor->Set(0.0);
						// launcher->SetRun(false); // We want it to stop running
					}
					DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
					if(ElevatorEncoder==NULL){
						dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "ERR");
					}
					else
					dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "Elevator Encoder:%4d",ElevatorEncoder->Get());
					dsLCD->UpdateLCD();
					if(ElevatorStick->GetRawButton(6))
						ElevatorEncoder->Reset();
					if(ElevatorStick->GetRawButton(7))
						HomeItFlag=true;
					if(HomeItFlag)
						HomeIt(.25, &HomeItFlag, false); // Home at 1/4 speed
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


void RobotElevator::HomeIt(float MotorSpeed, bool *flag, bool SlowDown)
{
		// This tracks the first calls in a elevator cycle to this function
		// necessary for reseting the encoder at beginning of cycle.
		static bool isFirstTime = true;
		
		if(isFirstTime){
			ElevatorEncoder->Reset(); // reset encoder the first time the function is called.
			isFirstTime=false;
		}
		
		if(SlowDown){// To use slowdown, pass true into the slowdown parameter when calling HomeIt()
				if(ElevatorEncoder->Get()<=800)
					ElevatorMotor->Set(MotorSpeed);
				
				else if(ElevatorEncoder->Get()<=950)
					ElevatorMotor->Set(MotorSpeed/2.0);
				
				else
					ElevatorMotor->Set(MotorSpeed/4.0);
		}
		else
			ElevatorMotor->Set(MotorSpeed);// Otherwise, just use motorspeed.
		
		if(HomeSwitch->IsTripped()){
			ElevatorMotor->Set(0);
			*flag=false;
			isFirstTime=true; 
			// This will reset homeit to reset the encoder again the next cycle.
		}
		
}

