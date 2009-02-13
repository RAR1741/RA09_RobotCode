#include "Harvester.h"

RobotHarvester::RobotHarvester(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel)
{
	HarvesterMotor = new Jaguar(MotorSlot, MotorChannel);
	HarvesterMotorCurrent = new AnalogChannel(CurrentSlot, CurrentChannel);
	HarvesterButton = 0;
	EjecterButton = 0;
	State = 0;
	AutoMode = 0;
}

RobotHarvester::~RobotHarvester()
{
	HarvesterMotor = NULL;
	HarvesterMotorCurrent = NULL;
	HarvesterStick = NULL;
	EjecterStick = NULL;
	HarvesterToggle = NULL;
	EjecterToggle = NULL;
}

UINT32 RobotHarvester::GetState(void)
{
	return State;
}

void RobotHarvester::SetState(UINT32 NewState)
{
	State = NewState;
}

UINT32 RobotHarvester::GetAutoMode(void)
{
	return AutoMode;
}

void RobotHarvester::SetAutoMode(UINT32 NewAutoMode)
{
	AutoMode = NewAutoMode;
}

void RobotHarvester::SetHarvesterControls(Joystick *Stick, UINT32 Button) 
{ 
	HarvesterStick = Stick;
	HarvesterButton = Button;
	HarvesterToggle = new Toggle(Stick, Button);
}

void RobotHarvester::SetEjecterControls(Joystick *Stick, UINT32 Button)
{
	EjecterStick = Stick;
	EjecterButton = Button;
	EjecterToggle = new Toggle(Stick, Button);
}

void RobotHarvester::Init(void)
{
	State = 0;
	AutoMode = 0;
	HarvesterMotor->Set(0.0);
}

void RobotHarvester::Process(bool LoadElevator)
{
	/*
	if(LoadElevator)
		HarvesterAutoMode = HARV_AUTO_MODE_LOAD;
	if(EjectToggle)
		UserRequestEjectIdle = !UserRequestEjectIdle;
	*/
	/*
	if (true) { // manual mode
		if (RunStopToggle && !LastButtonValue) {
			UserRequestRunStop = !UserRequestRunStop;
		} else {
			// Maintain state
		}
		if (EjectToggle && !LastEjectValue) {
			UserRequestEjectIdle = !UserRequestEjectIdle;
		}

		int direction = (UserRequestEjectIdle ? 1 : -1);
		if (UserRequestRunStop) {
			Harvester_Motor.Set(.5 * direction);
		} else {
			Harvester_Motor.Set(0.0);
		}
	}
	
	LastButtonValue = RunStopToggle;
	LastEjectValue = EjectToggle;
	
	*/
	/*
	if (RunStopToggle) {
		Harvester_Motor.Set(.5);
	} else if (EjectToggle) {
		Harvester_Motor.Set(-.5);
	}
	*/
	if (HarvesterStick != NULL && EjecterStick != NULL){
		HarvesterToggle->UpdateState();
		EjecterToggle->UpdateState();

		if(1){// Put Manual/Auto if condition here. 
			// Manual Mode
			if (HarvesterToggle->GetOutput()){
				HarvesterMotor->Set(.5);
				EjecterToggle->Reset();
			}
			else{
				if(EjecterToggle->GetOutput()){
					HarvesterMotor->Set(-.5);
				}
				else{
					HarvesterMotor->Set(0.0);
				}
			}
		}
		else{
			// Auto mode code should be here.
		}
	}
}

