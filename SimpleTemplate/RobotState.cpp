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
	
	current_sensor_turret = 0.0;
	current_sensor_harvester = 0.0;
	current_sensor_elevator = 0.0;
	current_sensor_launch = 0.0;
	current_sensor_left = 0.0;
	current_sensor_right = 0.0;
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

void RobotState::SetUltrasonicVoltage(float volts)
{
	this->ultra_sensor = volts;
}
void RobotState::SetLeftCurrent(float amps)
{
	this->current_sensor_left = amps;
}
void RobotState::SetCurrent(enum RobotCurrentSensor cs, float amps)
{
	if (cs < 0 || cs >= RobotState::kNumCurrentSensors) return;
	else current_sensor_amps[cs] = amps;
	
	/*
	switch (cs)
	{
	case RobotState::ElevatorCurrent:
		this->current_sensor_elevator = amps;
		break;
	case RobotState::HarvesterCurrent:
		this->current_sensor_harvester = amps;
		break;
	case RobotState::LaunchWheelsCurrent:
		this->current_sensor_launch = amps;
		break;
	case RobotState::LeftSideDriverCurrent:
		this->current_sensor_left = amps;
		break;
	case RobotState::RightSideDriverCurrent:
		this->current_sensor_right = amps;
		break;
	case RobotState::TurretPositioningCurrent:
		this->current_sensor_turret = amps;
		break;
	default:
		// Should not do anything.
		break;
	}
	*/
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
	packet->m_ElevatorEncoder = quad_encoder_counts[RobotState::ElevatorEncoder];
	packet->m_gyroAngle = gyro_integrated_angle;
	packet->m_gyroTemp = gyro_temp;
	
	packet->m_accelX = accel_inputs[RobotState::XAxis];
	packet->m_accelY = accel_inputs[RobotState::YAxis];
	packet->m_accelZ = accel_inputs[RobotState::ZAxis];
	
	packet->m_TurretPotentEncoderVoltage = turret_potentiometer;
	
	//packet->m_RightFollowerWheel = quad_encoder_counts[RobotState::RightFollow];
	
	//dashboardPacker.AddFloat();	// Elevator amps
	packet->m_elevator_amps = current_sensor_amps[RobotState::ElevatorCurrent];
	//packet->m_elevator_amps = this->current_sensor_elevator;
			//dashboardPacker.AddFloat();// launch amps
	packet->m_launch_amps = current_sensor_amps[RobotState::LaunchWheelsCurrent];
	//packet->m_launch_amps = this->current_sensor_launch;
			//dashboardPacker.AddFloat();// turret amps
	packet->m_turret_amps = current_sensor_amps[RobotState::TurretPositioningCurrent];
	//packet->m_turret_amps = this->current_sensor_turret;
			//dashboardPacker.AddFloat();// left drive amps
	packet->m_left_drive_amps = current_sensor_amps[RobotState::LeftSideDriverCurrent];
	//packet->m_left_drive_amps = this->current_sensor_left;
	
			//dashboardPacker.AddFloat();// right drive amps
	packet->m_right_drive_amps = current_sensor_amps[RobotState::RightSideDriverCurrent];
	//packet->m_right_drive_amps = this->current_sensor_right;
			//dashboardPacker.AddFloat();// harvester amps
	packet->m_harvester_amps = current_sensor_amps[RobotState::HarvesterCurrent];
	//packet->m_harvester_amps = this->current_sensor_harvester;

	packet->m_PWM_Elevator = this->pwm_outputs[RobotState::ElevatorOutput];
	packet->m_PWM_Harvester_Drive = this->pwm_outputs[RobotState::HarvesterOutput];
	packet->m_PWM_Launch = this->pwm_outputs[RobotState::LaunchWheelsOutput];
	packet->m_PWM_Left_Drive = this->pwm_outputs[RobotState::LeftSideDriveOutput];
	packet->m_PWM_Right_Drive = this->pwm_outputs[RobotState::RightSideDriveOutput];
	packet->m_PWM_Turret = this->pwm_outputs[RobotState::TurretPositioningOutput];
	
	packet->m_ElvCyclingFlag = ElevatorCycling;
	packet->m_ElvHomingFlag = ElevatorHoming;
	packet->m_ElvHomeSwitchFlag = ElevatorHomeSwitch;
	
	packet->m_ultrasonic = ultra_sensor;
}


void RobotState::ReportElevatorFlags(bool CyclingFlag, bool HomingFlag, bool HomeSwitch)
{
	ElevatorCycling = CyclingFlag;
	ElevatorHoming = HomingFlag;
	ElevatorHomeSwitch = HomeSwitch;
}
