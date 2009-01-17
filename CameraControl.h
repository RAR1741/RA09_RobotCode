#ifndef CAMERA_CONTROL__H
#define CAMERA_CONTROL__H
#include <nivision.h>

#include <ErrorBase.h>
#include <PCVideoServer.h>

class CameraControl 
{
public:
	CameraControl(void);
	~CameraControl(void);
private:
	PCVideoServer * m_Cam;
};
#endif
