#include "RobotState.h"

RobotState::RobotState()
{
	
}

RobotState::~RobotState()
{
	
}

void RobotState::SetQuadEncoder(enum RobotEncoder encoder, UINT32 count)
{
	if (encoder < 0 || encoder >= RobotState::kNumEncoders) return;
	else quad_encoder_counts[encoder] = count;
}

void RobotState::SetAccelerometerAxis(enum RobotAccelAxis axis, float x)
{
	if (axis < 0 || axis >= RobotState::kNumAccelAxis) return;
	else accel_inputs[axis] = x;
}

void RobotState::SetGyroData(float rate, float temp, float I_angle)
{
	gyro_rate = rate;
	gyro_temp = temp;
	gyro_integrated_angle = I_angle;
	
}

void RobotState::SetTurretPosition(float volts)
{
	turret_potentiometer = volts;
}
void RobotState::SetBatteryVoltage(float volts)
{
	battery_voltage = volts;
}

void RobotState::SetCurrent(enum RobotCurrentSensor cs, float amps)
{
	if (cs < 0 || cs >= RobotState::kNumCurrentSensors) return;
	else current_sensor_amps[cs] = amps;
}

void RobotState::SetPWMOutput(enum RobotOutput output, float power)
{
	if (output < 0 || output >= RobotState::kNumOutputs) return;
	else pwm_outputs[output] = power;
}

void RobotState::SetPneumaticDataGate(bool solA, bool solB)
{
	sol_A_Gate = solA;
	sol_B_Gate = solB;
}

void RobotState::SetPneumaticDataGrabber(bool solA, bool solB)
{
	sol_A_Grabber = solA;
	sol_B_Grabber = solB;
		
}


void RobotState::SetAirCompressorData(bool currentlyCompressing)
{
	air_compressing = currentlyCompressing;
}
