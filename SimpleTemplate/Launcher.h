#ifndef __LAUNCHER_H
#define __LAUNCHER_H
#include "wpilib.h"

class Launcher : public Jaguar
{
public:
	Launcher(UINT32 slot, UINT32 channel);
	~Launcher();
	void Update();
	void SetJoyStick(Joystick * thestick);
	void SetRun(bool runManrun);
private:
	UINT32 slot;
	UINT32 channel;
	Joystick * stick;
	bool shouldRun;
};
#endif
