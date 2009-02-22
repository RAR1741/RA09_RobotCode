#ifndef CAMERA_PID_SOURCE_H__
#define CAMERA_PID_SOURCE_H__

#include "LimitedPIDSource.h"

class CameraPIDSource : public LimitedPIDSource
{
public:
	CameraPIDSource();
	
	virtual void SetRelativePosition(double angle);
};

#endif
