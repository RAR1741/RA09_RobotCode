#ifndef POT_ENCODER_H__
#define POT_ENCODER_H__

#include "AnalogChannel.h"
//#include "LimitedPIDSource.h"
#include "CustomPIDSource.h"

class PotentiometerEncoder : public CustomPIDSource {
public:
	PotentiometerEncoder(UINT32 slot, UINT32 channel);
	virtual ~PotentiometerEncoder() {}
	
	static const float kEncoderRange = 1.9;
	
	float GetVoltage() { return encoder.GetVoltage(); }
protected:
	
	virtual void Poll(void);
	AnalogChannel encoder;
};

#endif
