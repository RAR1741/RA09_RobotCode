#ifndef CAMERA_PID_SOURCE_H__
#define CAMERA_PID_SOURCE_H__

#include "LimitedPIDSource.h"
#include "CustomPIDSource.h"

class CameraPIDSource : public CustomPIDSource
{
public:
	CameraPIDSource();
	
	virtual void SetRelativePosition(double angle);
};

#endif
