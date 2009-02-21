#include "IFF.h"


IFF::IFF()
{
	m_ds = DriverStation::GetInstance();
}

void IFF::DebugSetAlliance(DriverStation::Alliance al)
//void IFF::DebugSetAlliance(int al)
{
	this->ourSide = al;
}

void IFF::ResetToFieldControl(void)
{
	ourSide = m_ds->GetAlliance();
}

SecondColorPosition IFF::GetTargetConfiguration()
{
	switch (GetAlliance())
	{
	case DriverStation::kRed:
		return BELOW;
	case DriverStation::kBlue:
		return ABOVE;
	case DriverStation::kInvalid:	// Most likely our current case
	default:
		return BELOW;
	}
}

DriverStation::Alliance IFF::GetAlliance()
{
	return m_ds->GetAlliance();
}
