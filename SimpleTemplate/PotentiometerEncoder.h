#ifndef POT_ENCODER_H__
#define POT_ENCODER_H__

#include "AnalogChannel.h"
#include "LimitedPIDSource.h"

class PotentiometerEncoder : public LimitedPIDSource {
public:
	PotentiometerEncoder(UINT32 slot, UINT32 channel);
	virtual ~PotentiometerEncoder() {}
	
	float GetVoltage() { return encoder.GetVoltage(); }
protected:
	
	virtual void Poll(void);
	AnalogChannel encoder;
};

#endif
