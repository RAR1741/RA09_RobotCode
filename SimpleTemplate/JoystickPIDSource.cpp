#include "JoystickPIDSource.h"

JoystickPIDSource::JoystickPIDSource(void)
{
	m_Stick = NULL;
	m_Axis = Joystick::kXAxis;
	
	SetPIDGet(0.0);
}

void JoystickPIDSource::SetJoystick(Joystick *stick, Joystick::AxisType axis)
{
	m_Stick = stick;
	m_Axis = axis;
}

void JoystickPIDSource::Poll(void)
{
	if (m_Stick != NULL) {
		SetPIDGet(m_Stick->GetAxis(m_Axis));
	} else {
		SetPIDGet(0.0);
	}
}

