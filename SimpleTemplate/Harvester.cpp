#include "Harvester.h"
#include "DriverStationLCD.h"

RobotHarvester::RobotHarvester(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel)
{
	HarvesterMotor = new Jaguar(MotorSlot, MotorChannel);
	HarvesterMotorCurrent = new AnalogChannel(CurrentSlot, CurrentChannel);
	RunStopButton = 0;
	CollectEjectButton = 0;
	State = 0;
	AutoMode = MODE_MANUAL;
	HarvesterFull = false;
	HarvesterMode = 0;
	HarvesterMotorVoltage = 0.0;
}

RobotHarvester::~RobotHarvester()
{
	HarvesterMotor = NULL;
	HarvesterMotorCurrent = NULL;
	RunStopStick = NULL;
	CollectEjectStick = NULL;
	RunStopToggle = NULL;
	CollectEjectToggle = NULL;
	TheGate = NULL;
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

float RobotHarvester::GetHarvesterMotorVoltage(void)
{
	return HarvesterMotorVoltage;
}

void RobotHarvester::SetCollectEjectControls(Joystick *Stick, UINT32 Button) 
{ 
	RunStopStick = Stick;
	RunStopButton = Button;
	RunStopToggle = new Toggle(Stick, Button);
}

void RobotHarvester::SetRunStopControls(Joystick *Stick, UINT32 Button)
{
	CollectEjectStick = Stick;
	CollectEjectButton = Button;
	CollectEjectToggle = new Toggle(Stick, Button);
}

void RobotHarvester::SetGateControls(GateController *Gate)
{
	TheGate = Gate;
}

void RobotHarvester::Init(void)
{
	State = 0;
	AutoMode = MODE_MANUAL;
	HarvesterMotor->Set(0.0);
}

void RobotHarvester::Process(bool LoadElevator)
{
	RunStopToggle->UpdateState();
	CollectEjectToggle->UpdateState();

	HarvesterMotorVoltage = HarvesterMotorCurrent->GetVoltage();
	DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
	
	dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Current:%7.1f", HarvesterMotorVoltage);
//	dsLCD->Printf(DriverStationLCD::kUser_Line2, 8, ":%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line3, 17, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line4, 7, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line5, 4, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line6, 10, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "%-7.1f", GetClock());
	dsLCD->UpdateLCD();
	
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
	if (RunStopStick != NULL && CollectEjectStick != NULL){
		if (RunStopToggle->GetOutput()){
			HarvesterMotor->Set(COLLECT_MOTOR_SPEED);
			CollectEjectToggle->Reset();
		}
		else{
			if(CollectEjectToggle->GetOutput()){
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
	State = HarvesterFull | 
			(CollectEjectToggle->GetOutput() << 1) |
			(RunStopToggle->GetOutput() << 2) |
			(LoadElevator << 3) |
			(HarvesterMode << 4);
	
	switch(State){
		case 0:		// Run/Stop = Stop
		case 1:
		case 2:
		case 3:
		case 5:
		case 8:
		case 9:
		case 10:
		case 11:
		case 16:
		case 17:
		case 18:
		case 19:
		case 21:
		case 24:
		case 25:
		case 26:
		case 27:
		case 32:
		case 33:
		case 34:
		case 35:
		case 36:
		case 37:
		case 40:
		case 41:
		case 42:
		case 43:
		case 48:
		case 49:
		case 50:
		case 51:
		case 53:	// Lost load command and are full to go to idle
		case 56:
		case 57:
		case 58:
		case 59:
			HarvesterMotor->Set(MOTOR_STOP);
			TheGate->Close();
			HarvesterMode = 0;
			break;
		
		case 4:		// Not Full - In collect mode
		case 20:	// Not Full - In collect mode
		case 52:	// Lost load elevator command so default to collect mode
			HarvesterMotor->Set(COLLECT_MOTOR_SPEED);
			TheGate->Close();
			HarvesterMode = 1;
			break;
			
		case 6:		// Received Eject signal while running
		case 7:		// Received Eject signal while running
		case 22:	// Received Eject signal while running
		case 23:	// Received Eject signal while running
		case 38:	// Eject
		case 39:	// Eject
		case 54:	// Eject
		case 55:	// Eject
			HarvesterMotor->Set(EJECT_MOTOR_SPEED);
			TheGate->Close();
			HarvesterMode = 2;
			break;
			
		case 12:	// Received Load Elevator command
		case 13:	// Received Load Elevator command
		case 14:	// Received Load Elevator command
		case 15:	// Received Load Elevator command
		case 28:	// Received Load Elevator command
		case 29:	// Received Load Elevator command
		case 30:	// Received Load Elevator command
		case 31:	// Received Load Elevator command
		case 44:	// Received Load Elevator command
		case 45:	// Received Load Elevator command
		case 46:	// Received Load Elevator command
		case 47:	// Received Load Elevator command
		case 60:	// Received Load Elevator command
		case 61:	// Received Load Elevator command
		case 62:	// Received Load Elevator command
		case 63:	// Received Load Elevator command
			HarvesterMotor->Set(COLLECT_MOTOR_SPEED);
			TheGate->Open();
			HarvesterMode = 3;
			break;
			
		default:
			break;
	}
}


