#include "EmptyCellCatcher.h"

CellCatcher::CellCatcher()
{

}

void CellCatcher::Set(bool theBoolean)
{
	solenoidA->Set(theBoolean);
	solenoidB->Set(!theBoolean);
}

CellCatcher::~CellCatcher()
{
	delete solenoidA;
	delete solenoidB;
}

void CellCatcher::Init(UINT32 slotA, UINT32 channelA, UINT32 slotB, UINT32 channelB)
{
	solenoidA = new Solenoid(slotA, channelA);
	solenoidB = new Solenoid(slotB, channelB);
}
