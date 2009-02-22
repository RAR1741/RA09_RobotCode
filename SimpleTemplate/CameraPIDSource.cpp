#include "CameraPIDSource.h"

CameraPIDSource::CameraPIDSource()
{
	// Thanks to inheritance, this is largely a stateless module
	SetRelativePosition(0.0);
	
}

void CameraPIDSource::SetRelativePosition(double angle)
{
	SetPIDGet(angle); // Just set up for the rest of the code.
}

