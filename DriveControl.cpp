#include "DriveControl.h"

DriveControl::DriveControl():
leftMotor(4,2),
rightMotor(4,1)
{}
DriveControl::~DriveControl()
{}

void DriveControl::Left( float speed )
{
	leftMotor.SetRaw((unsigned char)((speed + 1) * 127));
}

void DriveControl::Right( float speed )
{
	rightMotor.SetRaw((unsigned char)((speed + 1) * 127));
}

void DriveControl::SingleJoystickControl(float xAxis,float yAxis)
{
	DriveControl.Left(yAxis+xAxis);
	DriveControl.Right(yAxis-xAxis);
}