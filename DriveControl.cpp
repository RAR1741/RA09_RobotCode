#include "DriveControl.h"

DriveControl::DriveControl():
leftMotor(4,2),
rightMotor(4,1)
{}
DriveControl::~DriveControl()
{}

void DriveControl::Left( float speed )
{
	if (speed < -1)
		speed = -1;
	if (speed > 1)
		speed = 1;
	leftMotor.SetRaw((unsigned char)((speed + 1) * 127));
}

void DriveControl::Right( float speed )
{
	if (speed < -1)
		speed = -1;
	if (speed > 1)
		speed = 1;
	rightMotor.SetRaw((unsigned char)((speed + 1) * 127));
}