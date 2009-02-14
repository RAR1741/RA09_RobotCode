#include "Launcher.h"

Launcher::Launcher(UINT32 Slot, UINT32 Channel) :
	Jaguar(Slot, Channel)
	{
		slot = Slot;
		channel = Channel;
		// stick = joystick;
		shouldRun=false;
	}

void Launcher::Update()
{
	if(stick!=NULL && shouldRun)
		Set(-((-stick->GetZ()+1.0)/2.0));
	else
		Set(0.0);
}

void Launcher::SetJoyStick(Joystick * thestick)
{
	stick = thestick;
}

void Launcher::SetRun(bool runManrun)
{
	shouldRun=runManrun;
}
