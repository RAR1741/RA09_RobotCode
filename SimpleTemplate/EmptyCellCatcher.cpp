#include "EmptyCellCatcher.h"
#include "DriverStationLCD.h"

CellCatcher::CellCatcher()
{

}

void CellCatcher::Set(bool theBoolean)
{
	DriverStationLCD * dlcd = DriverStationLCD::GetInstance();
	dlcd->Printf(DriverStationLCD::kUser_Line4, 7, "%1d", theBoolean);
	solenoidA->Set(theBoolean);
	solenoidB->Set(!theBoolean);
	light->Set(!theBoolean);
	// this is actually correct.
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
	light = new DigitalOutput(6,13);
	light->Set(1);
}
