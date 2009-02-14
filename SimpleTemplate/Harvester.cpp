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
	switch (AutoMode)
	{
		case MODE_MANUAL:
			ProcessManual();
			break;
			
		case MODE_SEMI_AUTO:
			ProcessSemiAuto(LoadElevator);
			break;
			
		case MODE_AUTO:
			ProcessAuto(LoadElevator);
			break;
			
		default:
			break;
	}
}

void RobotHarvester::ProcessManual(void)
{
	if (HarvesterStick != NULL && EjecterStick != NULL){
		HarvesterToggle->UpdateState();
		EjecterToggle->UpdateState();

		if (HarvesterToggle->GetOutput()){
			HarvesterMotor->Set(COLLECT_MOTOR_SPEED);
			EjecterToggle->Reset();
		}
		else{
			if(EjecterToggle->GetOutput()){
				HarvesterMotor->Set(EJECT_MOTOR_SPEED);
			}
			else{
				HarvesterMotor->Set(0.0);
			}
		}
	}
}

void RobotHarvester::ProcessSemiAuto(bool LoadElevator)
{
}

void RobotHarvester::ProcessAuto(bool LoadElevator)
{
	switch(AutoMode){
		default:
			break;
	}
}


