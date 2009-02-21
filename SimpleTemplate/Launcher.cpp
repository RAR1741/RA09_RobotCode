#include "Launcher.h"
#include "DriverStationLCD.h"

void Launcher::Init(UINT32 Slot, UINT32 Channel)
	{
		slot = Slot;
		channel = Channel;
		// stick = joystick;
		shouldRun=false;
		Status = false;
		motor = new Jaguar(Slot, Channel);
		LaunchEncoder = new Encoder(4, 9, 4, 10, false);
		LaunchEncoder->Start();
		// LaunchEncoder->Reset();
		launchWheelsCurrent = new CurrentSensor();
		launchWheelsCurrent->Init(2, 2, 2.5, CurrentSensor::m_30Amp);
	}

void Launcher::Update()
{
	if(stick!=NULL && shouldRun)
		motor->Set(-((-stick->GetZ()+1.0)/2.0));
	else
		motor->Set(0.0);
	
	if(launchWheelsCurrent->GetCurrent()>=10)
		Status = true;
	else Status = false;
	
	DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
    dsLCD->Printf(DriverStationLCD::kUser_Line3, 12, "LC:%2.2f",launchWheelsCurrent->GetCurrent());
    dsLCD->UpdateLCD();
	//DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
	/*dsLCD->Printf(DriverStationLCD::kUser_Line3, 9, "LEN:%4d",LaunchEncoder->Get());
	*/
}

void Launcher::SetJoyStick(Joystick * thestick)
{
	stick = thestick;
}

void Launcher::SetRun(bool runManrun)
{
	shouldRun=runManrun;
}

INT32 Launcher::GetEncoderVal()
{
	return LaunchEncoder->Get();
}

bool Launcher::GetStatus()
{
	return Status;
}

double Launcher::GetCurrentVal()
{
	return launchWheelsCurrent->GetCurrent();
}
