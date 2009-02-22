#include "Harvester.h"
#include "DriverStationLCD.h"


RobotHarvester::RobotHarvester(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel)
{
	HarvesterMotor = new Jaguar(MotorSlot, MotorChannel);
	HarvesterMotorCurrent = new CurrentSensor();
	HarvesterMotorCurrent->Init(CurrentSlot, CurrentChannel, 2.6, CurrentSensor::m_20Amp);
	RunStopButton = 0;
	CollectEjectButton = 0;
	State = 0;
	AutoMode = MODE_MANUAL;
	HarvesterFull = false;
	HarvesterMode = 0;
	HarvesterMotorCurrentVal = 0.0;
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
	return HarvesterMotorCurrentVal;
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
	CurrentPeakIgnoreTimer = new Timer();
	ClearHarvBeltTimer = new Timer();
	
}

void RobotHarvester::SetFull(bool isFull)
{
	HarvesterFull = isFull;
}

void RobotHarvester::Process(bool LoadElevator, bool RunStop)
{
	RunStopToggle->UpdateState();
	CollectEjectToggle->UpdateState();


	DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
	
	dsLCD->Printf(DriverStationLCD::kUser_Line6, 1, "HC:%3.1f", HarvesterMotorCurrentVal);
//	dsLCD->Printf(DriverStationLCD::kUser_Line2, 8, ":%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line3, 17, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line4, 7, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line5, 4, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line6, 10, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "%-7.1f", GetClock());
	dsLCD->UpdateLCD();
	dsLCD = NULL;
	
	switch (AutoMode)
	{
		case MODE_MANUAL:
			ProcessManual();
			break;
			
		case MODE_SEMI_AUTO:
			ProcessSemiAuto(LoadElevator);
			break;
			
		case MODE_AUTO:
			ProcessAuto(LoadElevator, RunStop);
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

void RobotHarvester::ProcessAuto(bool LoadElevator, bool RunStop)
{
	int OldState = State;
	int OldHarvesterMode = HarvesterMode;
	State = HarvesterFull | 
			(CollectEjectToggle->GetOutput() << 1) |
			(RunStop << 2) |
			(LoadElevator << 3) |
			(HarvesterMode << 4);
	
	DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
	

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
		case 13:
		case 16:
		case 17:
		case 18:
		case 19:
		
		case 21:
		case 24:
		case 25:
		case 26:
		case 27:
		case 29:
		case 32:
		case 33:
		case 34:
		case 35:
		
		case 37:
		case 40:
		case 41:
		case 42:
		case 43:
		case 45:
		case 48:
		case 49:
		case 50:
		case 51:
		case 53:	// Lost load command and are full to go to idle
		case 56:
		case 57:
		case 58:
		case 59:
		case 61:
			HarvesterMode = 0;
			break;
		
		case 4:		// Not Full - In collect mode
		case 20:
			// Not Full - In collect mode
		case 36:
		case 52:	// Lost load elevator command so default to collect mode;
		
			HarvesterMode = 1;
			break;
			
		case 6:		// Received Eject signal while running
		case 7:		// Received Eject signal while running
		case 14:
		case 15:
		case 22:	// Received Eject signal while running
		case 23:	// Received Eject signal while running
		case 30:
		case 31:
		case 38:	// Eject
		case 39:	// Eject
		case 46:
		case 47:
		case 54:	// Eject
		case 55:	// Eject
		case 62:
		case 63:
			HarvesterMode = 2;
			break;
			
		case 12:	// Received Load Elevator command
			// Received Load Elevator command
			// Received Load Elevator command
			// Received Load Elevator command
		case 28:	// Received Load Elevator command
			// Received Load Elevator command
			// Received Load Elevator command
			// Received Load Elevator command
		case 44:	// Received Load Elevator command
		// Received Load Elevator command
			// Received Load Elevator command
			// Received Load Elevator command
		case 60:	// Received Load Elevator command
			// Received Load Elevator command
			// Received Load Elevator command
			// Received Load Elevator command

			HarvesterMode = 3;
			break;
			
		default:
			break;
	}
	
	switch(HarvesterMode)
	{
	 case 0:
		 	TheGate->Close();
			HarvesterMotor->Set(MOTOR_STOP);
			CurrentPeakIgnoreTimer->Stop();
			CurrentPeakIgnoreTimer->Reset();
		 break;
	
	 case 1:
		 	TheGate->Close();
			CurrentPeakIgnoreTimer->Start();
		 	if(CurrentPeakIgnoreTimer->Get()>.5){
			HarvesterMotorCurrentVal = HarvesterMotorCurrent->GetCurrent();
			HarvesterFull = HarvesterMotorCurrentVal > 12;
		 	}
		 break;
	 case 2:
		 	TheGate->Close();
			HarvesterMotor->Set(EJECT_MOTOR_SPEED);

		 break;
	 case 3:
		 	TheGate->Open();
			HarvesterMotor->Set(COLLECT_MOTOR_SPEED);
			
			CurrentPeakIgnoreTimer->Start();
		 	if(CurrentPeakIgnoreTimer->Get()>.5){
			HarvesterMotorCurrentVal = HarvesterMotorCurrent->GetCurrent();
			HarvesterFull = HarvesterMotorCurrentVal > 12;
		 	}
//			if(HarvesterMotorCurrent->GetCurrent())
		 break;
		 
	 default:
		 break;
		 
		
	}
	dsLCD->Printf(DriverStationLCD::kUser_Line4, 10, "H1:%d  H2:%d", OldHarvesterMode, HarvesterMode);
	dsLCD->Printf(DriverStationLCD::kUser_Line5, 10, "S1:%2d  S2:%2d", OldState, State);
	dsLCD->Printf(DriverStationLCD::kUser_Line6, 5, "HMC: %2.2f", HarvesterMotorCurrent->GetCurrent());
//	dsLCD->Printf(DriverStationLCD::kUser_Line2, 8, ":%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line3, 17, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line4, 7, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line5, 4, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line6, 10, "%7.1f", GetClock());
//	dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "%-7.1f", GetClock());
	dsLCD->UpdateLCD();
	dsLCD = NULL;
}


