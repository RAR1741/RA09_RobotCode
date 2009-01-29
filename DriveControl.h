#ifndef DRIVECONTROL__
#define DRIVECONTROL__
#include "WPILib.h"
// #include <cmath.h>


class DriveControl
{
public:
	DriveControl(); 	
	~DriveControl();
	void Left(float);
	void Right(float);
	void SingleJoystickControl(float,float);
private:
	PWM leftMotor;
	PWM rightMotor;
};

#endif
