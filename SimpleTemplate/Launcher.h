#ifndef __LAUNCHER_H
#define __LAUNCHER_H
#include "WPIlib.h"
#include "CurrentSensor.h"

class Launcher
{
public:
	void Init(UINT32 slot, UINT32 channel);
	//~Launcher();
	void Update();
	void SetJoyStick(Joystick * thestick);
	void SetRun(bool runManrun);
	bool GetStatus();
	double GetCurrentVal();
	INT32 GetEncoderVal();
	
	
	static const double kThreshold = 32.0;
	static const double kClipPoint = 35.0;
	static const double kCorrectionFactor = - 1.0 / 3.0;
	
private:
	UINT32 slot;
	UINT32 channel;
	Joystick * stick;
	Jaguar * motor;
	Encoder * LaunchEncoder;
	CurrentSensor * launchWheelsCurrent;
	bool shouldRun;
	bool Status;
};
#endif
