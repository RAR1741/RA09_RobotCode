/*********************************************************************
 * IFF.h
 * 
 * This file simply contains an IFF class that abstracts and simplifies
 * some Identification Frend, Foe (acronym courtesy of U.S. DOD)
 * functionality. I got tired of writing it directly in Turret.cpp
 *********************************************************************/

#ifndef IFF_H__
#define IFF_H__

#include "Target.h"			// For hooks into the color tracking algos
#include "DriverStation.h"	// For our *current* alignment :)

class IFF
{
public:
	IFF();
	
	// This sets our robot's "operating" alliance for pre-competition
	// war games.
	void DebugSetAlliance(DriverStation::Alliance side);
	// This un-does it.
	void ResetToFieldControl();
	
	SecondColorPosition GetTargetConfiguration();
	DriverStation::Alliance GetAlliance();
	
private:
	DriverStation *m_ds;
	DriverStation::Alliance ourSide;
};

#endif
