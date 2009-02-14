#include "GateController.h"

GateController::GateController(void)
{
	solenoidA = new Solenoid(8,1);
	solenoidB = new Solenoid(8,2);
}

void GateController::Set(bool theBoolean)
{
	solenoidA->Set(theBoolean);
	solenoidB->Set(!theBoolean);
}

GateController::~GateController(void)
{
	delete solenoidA;
	delete solenoidB;
}

void GateController::Open(void)
{
	Set(true);
}

void GateController::Close(void)
{
	Set(false);
}

/*
void CellCatcher::Init(UINT32 slotA, UINT32 channelA, UINT32 slotB, UINT32 channelB)
{
	solenoidA = new Solenoid(slotA, channelA);
	solenoidB = new Solenoid(slotB, channelB);
}
*/
