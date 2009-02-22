#include "CustomPIDSource.h"

CustomPIDSource::CustomPIDSource()
{
	m_pid_get = 0.0;
}

double CustomPIDSource::PIDGet()
{
	Poll();
	return m_pid_get;
}

void CustomPIDSource::Poll()
{
	// NOP in this class.
}

void CustomPIDSource::SetPIDGet(double foo)
{
	m_pid_get = foo;
}


// End
