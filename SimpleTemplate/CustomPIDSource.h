#ifndef CUSTOM_PID_SOURCE_H__
#define CUSTOM_PID_SOURCE_H__

#include "PIDSource.h"
class CustomPIDSource : public PIDSource
{
public:
	CustomPIDSource();
	
	virtual double PIDGet();
	
protected:
	
	// Override to insert custom processing to what is returned by PIDGet when
	// it is called. Useful for sensors and other continually updated simple
	// sources. Set the value via SetPIDGet (see below)
	virtual void Poll();
	
	// Use this to set the value that PIDGet will return. Useful for code that
	// will use other software as feedback for the PID loop.
	virtual void SetPIDGet(double foo);
	
private:
	double m_pid_get;
};

#endif
