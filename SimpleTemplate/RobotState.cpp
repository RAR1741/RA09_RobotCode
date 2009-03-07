#include "RobotState.h"

RobotState::RobotState()
{
	//INT32 quad_encoder_counts[4];
	for (int i = 0; i < RobotState::kNumEncoders; i++) {
		quad_encoder_counts[i] = 0;
	}
	
	//float accel_inputs[3];
	for (int i = 0; i < RobotState::kNumAccelAxis; i++) {
		accel_inputs[i] = 0.0;
	}
	
	//float gyro_rate;
	gyro_rate = 0.0;
	
	//float gyro_temp;
	gyro_temp = 0.0;
	
	//float gyro_integrated_angle;
	gyro_integrated_angle = 0.0;
	
	//float turret_potentiometer;
	turret_potentiometer = 0.0;
	
	//float battery_voltage;
	battery_voltage = 0.0;
	
	//float current_sensor_amps[6];
	for (int i = 0; i < RobotState::kNumCurrentSensors; i++) {
		current_sensor_amps[i] = 0.0;
	}
	
	//float pwm_outputs[6];
	for (int i = 0; i < RobotState::kNumOutputs; i++) {
		pwm_outputs[i] = 0.0;
	}
	
	//bool sol_A_Gate;
	sol_A_Gate = false;
	//bool sol_B_Gate;
	sol_B_Gate = false;
	//bool sol_A_Grabber;
	sol_A_Grabber = false;
	//bool sol_B_Grabber;
	sol_B_Grabber = false;
	
	//bool air_compressing;
	air_compressing = false;
}

RobotState::~RobotState()
{
	
}

void RobotState::SetQuadEncoder(enum RobotEncoder encoder, INT32 count, float velocity)
{
	if (encoder < 0 || encoder >= RobotState::kNumEncoders) return;
	else {
		quad_encoder_counts[encoder] = count;
		quad_encoder_vels[encoder] = velocity;
	}
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

void RobotState::SetTurretPosition(float volts, float velocity)
{
	turret_potentiometer = volts;
	turret_velocity = velocity;
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

void RobotState::PackData(DashboardDataFormat *packet)
{
//	
//	INT32 quad_encoder_counts[4];
//			float accel_inputs[3];
//			float gyro_rate;
//			float gyro_temp;
//			float gyro_integrated_angle;
//			float turret_potentiometer;
//			float battery_voltage;
//			float current_sensor_amps[6];
//			float pwm_outputs[6];
//			bool sol_A_Gate;
//			bool sol_B_Gate;
//			bool sol_A_Grabber;
//			bool sol_B_Grabber;
//
//			bool air_compressing;
	
	packet->m_LM_QuadEncoder = quad_encoder_counts[RobotState::LeftMotor];
	packet->m_LMWheelEncoder = quad_encoder_counts[RobotState::LeftFollow];
	packet->m_LaunchEncoder = quad_encoder_counts[RobotState::LaunchWheelsOutput];
	packet->m_RMWheelEncoder = quad_encoder_counts[RobotState::RightFollow];
	packet->m_RM_QuadEncoder = quad_encoder_counts[RobotState::RightMotor];
	
	packet->m_gyroAngle = gyro_integrated_angle;
	packet->m_gyroTemp = gyro_temp;
	
	packet->m_accelX = accel_inputs[RobotState::XAxis];
	packet->m_accelY = accel_inputs[RobotState::YAxis];
	packet->m_accelZ = accel_inputs[RobotState::ZAxis];
	
	packet->m_TurretPotentEncoderVoltage = turret_potentiometer;
	
	//packet->m_RightFollowerWheel = quad_encoder_counts[RobotState::RightFollow];
	
	
	
}
