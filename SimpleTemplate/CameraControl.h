#ifndef CAMERA_CONTROL__H
#define CAMERA_CONTROL__H
#include <nivision.h>
#include "CameraControl.h"
#include "DashboardDataFormat.h"
#include "vxWorks.h"
#include "AxisCamera.h"
#include "BaeUtilities.h"
#include "FrcError.h"
#include "PCVideoServer.h"
#include <ErrorBase.h>

class CameraControl : public PCVideoServer
{
public:
	CameraControl(void);
	~CameraControl(void);
};
#endif

