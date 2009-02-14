#ifndef __COLLECTOR_H__
#define __COLLECTOR_H__

#include "WPIlib.h"

class CellCatcher : public Solenoid
{
	CellCatcher(UINT32 slot, UINT32 slot);
	~CellCatcher();
};

#endif