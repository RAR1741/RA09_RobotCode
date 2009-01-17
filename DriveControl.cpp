#include "DriveControl.h"

void DriveControl::Left()
{
	PWM motor1(4,2);
	motor1.SetSpeed(.5);
}

void DriveControl::Right()
{
	PWM motor2(4,1);
	motor2.SetSpeed(.5);
}