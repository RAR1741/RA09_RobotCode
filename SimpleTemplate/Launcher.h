#ifndef __LAUNCHER_H
#define __LAUNCHER_H
#include "WPIlib.h"

class Launcher
{
public:
	void Init(UINT32 slot, UINT32 channel);
	//~Launcher();
	void Update();
	void SetJoyStick(Joystick * thestick);
	void SetRun(bool runManrun);
	bool GetStatus();
	INT32 GetLauncherEncoderVal();
private:
	UINT32 slot;
	UINT32 channel;
	Joystick * stick;
	Jaguar * motor;
	Encoder * LaunchEncoder;
	bool shouldRun;
	bool Status;
};
#endif
