#ifndef ROBOT_STATE_H__
#define ROBOT_STATE_H__

#include "WPILib.h"

class RobotState {
public:
	RobotState();
	~RobotState();
	
	/////////////////////////////////////////////////////////////////////
	// Sensor states. Local to the robot.
	
	// Log ALL sensors. That's right. ALL of them.
	// These enumerate the different sensors of each type.
	enum Encoders { LeftMotor = 0, RightMotor, LeftFollow, RightFollow };
	enum AccelAxis { XAxis = 0, YAxis, ZAxis };
	enum CurrentSensor { ElevatorCurrent = 0, LaunchWheelsCurrent, 
		TurretPositioningCurrent, LeftSideDriverCurrent, RightSideDriverCurrent,
		HarvesterCurrent};
	enum RobotOutput { ElevatorOutput = 0, LaunchWheelsOutput, TurretPositioningOutput,
		LeftSideDriveOutput, RightSideDriveOutput, HarvesterOutput };
	};
	
	
	void SetQuadEncoder(enum Encoder encoder, UINT32 count);
	void SetAccelerometerAxis(enum AccelAxis axis, UINT32 count);
	void SetGyroData(float rate, float temp, float I_angle);
	void SetTurretPosition(float volts);
	void SetBatteryVoltage(float volts);
	
	void SetCurrent(enum CurrentSensor cs, float amps);
	
	void SetPWMOutput(enum RobotOutput output, float power);
	
	void SetPneumaticData(bool solA, bool solB);
	
	void SetAirCompressorData(bool currentlyCompressing);
	
	/////////////////////////////////////////////////////////////////////
private:
	
};

#endif