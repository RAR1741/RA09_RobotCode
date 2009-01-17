#include "UIControl.h"

// SingleJoystickControl(MyStick.GetRawAxis(asdf),MyStick.GetRawAxis(asdf));
// TYPES

void SingleJoystickControl(float xAxis,float yAxis)
{
	Drive.Left(xAxis+yAxis);
	Drive.Right(xAxis-yAxis);
}
