#include "Launcher.h"

void Launcher::Init(UINT32 Slot, UINT32 Channel)
	{
		slot = Slot;
		channel = Channel;
		// stick = joystick;
		shouldRun=false;
		motor = new Jaguar(Slot, Channel);
		LaunchEncoder = new Encoder(4, 9, 4, 10, false);
		LaunchEncoder->Start();
	}

void Launcher::Update()
{
	if(stick!=NULL && shouldRun)
		motor->Set(-((-stick->GetZ()+1.0)/2.0));
	else
		motor->Set(0.0);
}

void Launcher::SetJoyStick(Joystick * thestick)
{
	stick = thestick;
}

void Launcher::SetRun(bool runManrun)
{
	shouldRun=runManrun;
}

INT32 Launcher::GetLauncherEncoderVal()
{
	return LaunchEncoder->Get();
}

bool Launcher::GetStatus()
{
	return Status;
}
