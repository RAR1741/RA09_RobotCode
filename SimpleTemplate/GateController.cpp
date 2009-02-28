#include "GateController.h"
#include "DriverStationLCD.h"

GateController::GateController(void)
{
	solenoidA = new Solenoid(8,1);
	solenoidB = new Solenoid(8,2);
}

void GateController::Set(bool theBoolean)
{
	solenoidA->Set(theBoolean);
	solenoidB->Set(!theBoolean);
	if (theBoolean) {
			TurnOnGoggles();
	} else {
			TurnOffGoggles();
	}
}

GateController::~GateController(void)
{
	delete solenoidA;
	delete solenoidB;
}

void GateController::Open(void)
{
	Set(false);
}

void GateController::TurnOnGoggles(void)
{
	DriverStation *ds = DriverStation::GetInstance();
		
	ds->SetDigitalOut(1, true);
}

void GateController::TurnOffGoggles()
{
	DriverStation *ds = DriverStation::GetInstance();
	ds->SetDigitalOut(1, false);
}

void GateController::Close(void)
{
	Set(true);
}

/*
void CellCatcher::Init(UINT32 slotA, UINT32 channelA, UINT32 slotB, UINT32 channelB)
{
	solenoidA = new Solenoid(slotA, channelA);
	solenoidB = new Solenoid(slotB, channelB);
}
*/
