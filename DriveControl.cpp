#include <cmath>
#include "DriveControl.h"

using ::floor;


void DriveControl::Left( float speed )
{
	PWM motor1(4,2);
	motor1.SetRaw(RawFromFloat(speed));
}

void DriveControl::Right( float speed )
{
	PWM motor2(4,1);
	
	motor2.SetRaw(RawFromFloat(speed));
} 

UINT8 DriveControl::RawFromFloat(float speed)
{
	return static_cast<UINT8>(floor(speed * 128.0)) + 127;
}
