#ifndef __DashboardDataFormat_h__
#define __DashboardDataFormat_h__

#include "WPILib.h"

/**
 * This class is just an example of one way you could organize the data that you want
 * to send to the dashboard.  The PackAndSend method does all the work.  You could
 * put the packing code directly in your code, but this model protects you from
 * packing data in the wrong order throughout your code.
 * 
 * The data and the format of this structure are just an example.  It is written to
 * match the initial data format expected by the LabVIEW Dashboard project.  Feel free
 * to add data elements or remove them.  Just remember to make any changes consistently
 * between the LabVIEW "Dashboard Datatype" and the data that gets packed by this class.
 */
class DashboardDataFormat : public SensorBase
{
public:
	DashboardDataFormat(void);
	virtual ~DashboardDataFormat();
	float m_AnalogChannels[kAnalogModules][kAnalogChannels];
	UINT8 m_RelayFwd[kDigitalModules];
	UINT8 m_RelayRev[kDigitalModules];
	UINT16 m_DIOChannels[kDigitalModules];
	UINT16 m_DIOChannelsOutputEnable[kDigitalModules];
	UINT8 m_PWMChannels[kDigitalModules][kPwmChannels];
	UINT8 m_SolenoidChannels;
	INT32 m_LM_QuadEncoder;
	INT32 m_RM_QuadEncoder;
	INT32 m_LMWheelEncoder;
	INT32 m_RMWheelEncoder;
	INT32 m_LaunchEncoder;
	INT32 m_ElevatorEncoder;
	INT32 m_TurretEncoder;
	
	UINT32 m_TopBoundRect;
	UINT32 m_LeftBoundRect;
	UINT32 m_RightBoundRect;
	UINT32 m_BottomBoundRect;

	INT16 m_LeftMotorVoltage;
	INT16 m_RightMotorVoltage;
	INT16 m_launchWheelsCurrent;
	INT16 m_turretPositionCurrent;
	float m_gyroAngle;
	float m_gyroTemp;    // Added by HAM 2/6/9
	float m_accelX;
	float m_accelY;
	float m_accelZ;
	
	//dashboardPacker.AddFloat();	// Elevator amps
	float m_elevator_amps;
		//dashboardPacker.AddFloat();// launch amps
	float m_launch_amps;
		//dashboardPacker.AddFloat();// turret amps
	float m_turret_amps;
		//dashboardPacker.AddFloat();// left drive amps
	float m_left_drive_amps;
		//dashboardPacker.AddFloat();// right drive amps
	float m_right_drive_amps;
		//dashboardPacker.AddFloat();// harvester amps
	float m_harvester_amps;
	INT32 m_PrimaryMinHue;
	INT32 m_PrimaryMinSat;
	INT32 m_PrimaryMinLum;
	INT32 m_PrimaryMaxHue;
	INT32 m_PrimaryMaxSat;
	INT32 m_PrimaryMaxLum;
	
	INT32 m_SecondaryMinHue;
	INT32 m_SecondaryMinSat;
	INT32 m_SecondaryMinLum;
	INT32 m_SecondaryMaxHue;
	INT32 m_SecondaryMaxSat;
	INT32 m_SecondaryMaxLum;
	
	float m_TurretPotentEncoderVoltage;
	
	UINT16 m_LeftState;
	UINT16 m_RightState;
	UINT16 m_TurretState;
	
	INT32 m_LeftFollowerWheel;
	INT32 m_RightFollowerWheel;
	
	float m_LeftMotorVel;
	float m_RightMotorVel;
	
	float m_LeftFollowVel;
	float m_RightFollowVel;
	
	float m_LaunchVel;
	float m_TurretRotVel;
	
	float m_ElevatorVel;
	
	float m_ultrasonic;
	
	bool m_air_is_compressing;
	
	bool m_Gate_SolA;
	bool m_Gate_SolB;
	
	bool m_Grabber_SolA;
	bool m_Grabber_SolB;
	
	float m_PWM_Elevator;
	float m_PWM_Launch;
	float m_PWM_Turret;
	float m_PWM_Left_Drive;
	float m_PWM_Right_Drive;
	float m_PWM_Harvester_Drive;
	
	bool m_ElvHomingFlag;	// Indicates that elevator is homing
	bool m_ElvCyclingFlag;	// Indicates that elevator is cycling
	bool m_ElvHomeSwitchFlag;	// Inicates that elevator has homed
	//bool m_elevator_home_switch;
	
	float m_Joystick_X[3];
	float m_Joystick_Y[3];
	float m_Joystick_Z[3];
	
	UINT32 m_Joystick_Buttons[3];
	
	
	void PackAndSend(void);
private:
	DISALLOW_COPY_AND_ASSIGN(DashboardDataFormat);
	DriverStation *m_ds;
};

void UpdateDashboard(void);

#endif // __DashboardDataFormat_h__
