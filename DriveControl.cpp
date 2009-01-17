#include "DriveControl.h"

void DriveControl::Left( float speed )
{
	PWM motor1(4,2);
	motor1.SetSpeed(speed);
}

void DriveControl::Right( float speed )
{
	PWM motor2(4,1);
	motor2.SetSpeed(speed);
}