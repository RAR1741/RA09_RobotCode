#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

#include "WPIlib.h"

class CellCatcher
{
public:
	CellCatcher();
	~CellCatcher();
	void Set(bool theBoolean);
	void Init(UINT32 slotA, UINT32 channelA, UINT32 slotB, UINT32 channelB);
private:
	Solenoid * solenoidA;
	Solenoid * solenoidB;
	DigitalOutput *light;
};

#endif
