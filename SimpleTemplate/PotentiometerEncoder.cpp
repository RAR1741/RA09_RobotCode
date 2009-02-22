#include "PotentiometerEncoder.h"

PotentiometerEncoder::PotentiometerEncoder(UINT32 slot, UINT32 channel) : encoder(slot, channel)
{
	
}

void PotentiometerEncoder::Poll()
{
	SetPIDGet((encoder.GetVoltage() - 2.5) / PotentiometerEncoder::kEncoderRange);
}
