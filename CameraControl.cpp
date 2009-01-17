#include "CameraControl.h"

CameraControl::CameraControl(): 
m_Cam()
{
	m_Cam.Start();
}

CameraControl::~CameraControl()
{
	m_Cam.Stop();
}


