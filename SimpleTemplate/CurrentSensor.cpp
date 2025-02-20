#include "CurrentSensor.h"

void CurrentSensor::Init(int slot, int channel, double pbias, int Sensor)
{
	bias = pbias;
	theSensor = new AnalogChannel(slot, channel);
	sensorType = Sensor;
}

double CurrentSensor::GetVoltage()
{
	return theSensor->GetVoltage();
}

double CurrentSensor::GetCurrent()
{
	double current = theSensor->GetVoltage();
	current -= bias;
	switch(sensorType)
	{
		case m_5Amp:
			current /= 0.158;
			break;
		case m_20Amp:
			current /= 0.100;
			break;
		case m_30Amp:
			current /= 0.066;
			break;
	}
	return current;
}
