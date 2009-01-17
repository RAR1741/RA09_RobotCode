#include "CameraControl.h"

CameraControl::CameraControl()
{
	m_Cam = new PCVideoServer();
}

CameraControl::~CameraControl()
{
	delete m_Cam;
}
