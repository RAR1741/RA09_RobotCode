
#include "DashboardDataFormat.h"

DashboardDataFormat::DashboardDataFormat(void)
	: m_ds (DriverStation::GetInstance())
{
	
}

DashboardDataFormat::~DashboardDataFormat()
{
	
}

/**
 * Pack data using the correct types and in the correct order to match the
 * default "Dashboard Datatype" in the LabVIEW Dashboard project.
 */
void DashboardDataFormat::PackAndSend(void)
{
	Dashboard &dashboardPacker = m_ds->GetDashboardPacker();
	//UINT32 module;
	//UINT32 channel;

	// Pack identification handshake flag header thingy
	// Useful for picking out where "our" data begins.
	// See Mr. Meyer's Packet Buffer Inspector Utility Thingamajig.
	dashboardPacker.AddU8('*');
	dashboardPacker.AddU8('=');
	dashboardPacker.AddU8('=');
	dashboardPacker.AddU8('*');
	
	// Pack the analog modules
	// Don't pack them anymore. They're stupid.
//	for (module = 0; module < kAnalogModules; module++)
//	{
//		dashboardPacker.AddCluster();
//		for (channel = 0; channel < kAnalogChannels; channel++)
//		{
//			dashboardPacker.AddFloat(m_AnalogChannels[module][channel]);
//		}
//		dashboardPacker.FinalizeCluster();
//	}
//	// Pack the digital modules
//	for (module = 0; module < kDigitalModules; module++)
//	{
//		dashboardPacker.AddCluster();
//		dashboardPacker.AddU8(m_RelayFwd[module]);
//		dashboardPacker.AddU8(m_RelayRev[module]);
//		dashboardPacker.AddU16(m_DIOChannels[module]);
//		dashboardPacker.AddU16(m_DIOChannelsOutputEnable[module]);
//		dashboardPacker.AddCluster();
//		for(channel = 0; channel < kPwmChannels; channel++)
//		{
//			dashboardPacker.AddU8(m_PWMChannels[module][channel]);
//		}
//		dashboardPacker.FinalizeCluster();
//		dashboardPacker.FinalizeCluster();
//	}
	// Pack the solenoid module
	 
	// Elevator amps
	
	// Lanuch amps
	// Turret 
	// LEft drive
	// Right drive
	// Harvester
	
	
	dashboardPacker.AddU8(m_SolenoidChannels);
	
	// TODO Figure out if U32 and I32 are causing problems together	
	dashboardPacker.AddU32(m_TopBoundRect);
	dashboardPacker.AddU32(m_LeftBoundRect);
	dashboardPacker.AddU32(m_BottomBoundRect);
	dashboardPacker.AddU32(m_RightBoundRect);
	
	// PJF RED ALERT CODE: Add encoder data
	
	dashboardPacker.AddI32(m_LM_QuadEncoder);
	dashboardPacker.AddI32(m_RM_QuadEncoder);
	dashboardPacker.AddI32(m_LMWheelEncoder);
	dashboardPacker.AddI32(m_RMWheelEncoder);
	dashboardPacker.AddI32(0);// put cammera saturation here.
	
	dashboardPacker.AddI32(m_LaunchEncoder);
	dashboardPacker.AddI32(m_ElevatorEncoder);
	dashboardPacker.AddI32(m_TurretEncoder);
	dashboardPacker.AddI16(m_LeftMotorVoltage);
	dashboardPacker.AddI16(m_RightMotorVoltage);
	
	dashboardPacker.AddFloat(m_gyroAngle);
	dashboardPacker.AddFloat(m_gyroTemp);    // Added by HAM 2/6/9
	
	dashboardPacker.AddFloat(m_ultrasonic);
	
	dashboardPacker.AddFloat(m_accelX);
	dashboardPacker.AddFloat(m_accelY);
	dashboardPacker.AddFloat(m_accelZ);
	
	// Add target data
	// Add color target code
	/*
	dashboardPacker.AddI32(m_PrimaryMinHue);
	dashboardPacker.AddI32(m_PrimaryMinSat);
	dashboardPacker.AddI32(m_PrimaryMinLum);
	dashboardPacker.AddI32(m_PrimaryMaxHue);
	dashboardPacker.AddI32(m_PrimaryMaxSat);
	dashboardPacker.AddI32(m_PrimaryMaxLum);
	
	dashboardPacker.AddI32(m_SecondaryMinHue);
	dashboardPacker.AddI32(m_SecondaryMinSat);
	dashboardPacker.AddI32(m_SecondaryMinLum);
	dashboardPacker.AddI32(m_SecondaryMaxHue);
	dashboardPacker.AddI32(m_SecondaryMaxSat);
	dashboardPacker.AddI32(m_SecondaryMaxLum);
	*/
	dashboardPacker.AddU16(this->m_LeftState);
	dashboardPacker.AddU16(this->m_RightState);
	dashboardPacker.AddU16(this->m_TurretState);
	dashboardPacker.AddU8('*');
	dashboardPacker.AddU8('^');
	dashboardPacker.AddFloat(m_TurretPotentEncoderVoltage);
	//dashboardPacker.AddFloat(42.54);
	dashboardPacker.AddU8('^');
	dashboardPacker.AddU8('*');
	
	//dashboardPacker.AddFloat();	// Elevator amps
	/*
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
	*/
	dashboardPacker.AddFloat(m_elevator_amps);	// Elevator amps
	dashboardPacker.AddFloat(m_launch_amps);// launch amps
	dashboardPacker.AddFloat(m_turret_amps);// turret amps
	dashboardPacker.AddFloat(m_left_drive_amps);// left drive amps
	dashboardPacker.AddFloat(m_right_drive_amps);// right drive amps
	dashboardPacker.AddFloat(m_harvester_amps);// harvester amps
	
//	float m_PWM_Elevator;
//	float m_PWM_Launch;
//	float m_PWM_Turret;
//	float m_PWM_Left_Drive;
//	float m_PWM_Right_Drive;
//	float m_PWM_Harvester_Drive;
//	
	dashboardPacker.AddFloat(m_PWM_Elevator);
	dashboardPacker.AddFloat(m_PWM_Launch);
	dashboardPacker.AddFloat(m_PWM_Turret);
	dashboardPacker.AddFloat(m_PWM_Left_Drive);
	dashboardPacker.AddFloat(m_PWM_Right_Drive);
	dashboardPacker.AddFloat(m_PWM_Harvester_Drive);
	
	dashboardPacker.AddU8(m_ElvHomingFlag);
	dashboardPacker.AddU8(m_ElvCyclingFlag);
	dashboardPacker.AddU8(m_ElvHomeSwitchFlag);
	
	// Output all of the joystick data, each one at a time.
	for (int i = 0; i < 3; i++)
	{
		dashboardPacker.AddFloat(this->m_Joystick_X[i]);
		dashboardPacker.AddFloat(this->m_Joystick_Y[i]);
		dashboardPacker.AddFloat(this->m_Joystick_Z[i]);
		dashboardPacker.AddU32(this->m_Joystick_Buttons[i]);
	}
//	dashboardPacker.AddI32(m_LeftFollowerWheel);
//	dashboardPacker.AddI32(m_RightFollowerWheel);
	// Flush the data to the driver station.
	dashboardPacker.Finalize();
}
