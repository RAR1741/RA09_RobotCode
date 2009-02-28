#ifndef GATE_CONTROLLER_H__
#define GATE_CONTROLLER_H__

#include "WPILib.h"

class GateController
{
public:
	GateController(void);
	~GateController(void);
	
	void Set(bool theBoolean);
	//void Init(UINT32 slotA, UINT32 channelA, UINT32 slotB, UINT32 channelB);
	
	void Open(void);
	
	void Close(void);
	
	private:
		
		void TurnOnGoggles(void);
		void TurnOffGoggles(void);
		
		Solenoid * solenoidA;
		Solenoid * solenoidB;
};

#endif
