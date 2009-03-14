#ifndef ROBOT_STATE_H__
#define ROBOT_STATE_H__

#include "WPILib.h"

#include "DashboardDataFormat.h"

class RobotState {
public:
	RobotState();
	~RobotState();
	
	/////////////////////////////////////////////////////////////////////
	// Sensor states. Local to the robot.
	
	// Log ALL sensors. That's right. ALL of them.
	// These enumerate the different sensors of each type.
	enum RobotEncoder { LeftMotor = 0, RightMotor, LeftFollow, RightFollow, ElevatorEncoder };
	static const int kNumEncoders = 5;
	enum RobotAccelAxis { XAxis = 0, YAxis, ZAxis };
	static const int kNumAccelAxis = 3;
	enum RobotCurrentSensor { ElevatorCurrent = 0, LaunchWheelsCurrent, 
		TurretPositioningCurrent, LeftSideDriverCurrent, RightSideDriverCurrent,
		HarvesterCurrent};
	static const int kNumCurrentSensors = 6;
	enum RobotOutput { ElevatorOutput = 0, LaunchWheelsOutput, TurretPositioningOutput,
		LeftSideDriveOutput, RightSideDriveOutput, HarvesterOutput };
	static const int kNumOutputs = 6;

	//enum RobotJoystick
	
	
	void SetQuadEncoder(RobotEncoder encoder, INT32 count, float vel);
	void SetAccelerometerAxis(enum RobotAccelAxis axis, float x);
	void SetGyroData(float rate, float temp, float I_angle);
	void SetTurretPosition(float volts, float vel);
	void SetBatteryVoltage(float volts);
	
	void SetLeftCurrent(float amps);
	
	void SetCurrent(enum RobotCurrentSensor cs, float amps);
	
	void SetPWMOutput(enum RobotOutput output, float power);
	
	void SetPneumaticDataGate(bool solA, bool solB);
	
	void SetPneumaticDataGrabber(bool solA, bool solB);
	
	void SetAirCompressorData(bool currentlyCompressing);
	
	void ReportElevatorFlags(bool CyclingFlag, bool HomingFlag);
	//void SetJoystickState(Joystick *stick);
	
	void PackData(DashboardDataFormat *packet);
	/////////////////////////////////////////////////////////////////////
private:
	INT32 quad_encoder_counts[5];
	float quad_encoder_vels[5];
	float accel_inputs[3];
	float gyro_rate;
	float gyro_temp;
	float gyro_integrated_angle;
	float turret_potentiometer;
	float turret_velocity;
	float battery_voltage;
	float current_sensor_amps[6];
	float current_sensor_left;
	float current_sensor_right;
	float current_sensor_harvester;
	float current_sensor_elevator;
	float current_sensor_turret;
	float current_sensor_launch;
	float pwm_outputs[6];
	bool sol_A_Gate;
	bool sol_B_Gate;
	bool sol_A_Grabber;
	bool sol_B_Grabber;
	
	bool air_compressing;
	bool ElevatorCycling;
	bool ElevatorHoming;

};

#endif
