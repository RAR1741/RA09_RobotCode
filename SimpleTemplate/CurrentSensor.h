#ifndef CURRENT_SENSOR_H
#define CURRENT_SENSOR_H
#include "AnalogChannel.h"

class CurrentSensor
{
	public:
		enum{m_5Volt, m_20Volt, m_30Volt};
	void Init(int slot, int channel, double bias, int sensor);
	double GetVoltage();
	double GetCurrent();
	private:
		AnalogChannel * theSensor;
		double bias;
		int sensorType;
};
#endif
