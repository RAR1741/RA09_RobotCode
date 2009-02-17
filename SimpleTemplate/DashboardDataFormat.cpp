
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
	UINT32 module;
	UINT32 channel;

	// Pack the analog modules
	for (module = 0; module < kAnalogModules; module++)
	{
		dashboardPacker.AddCluster();
		for (channel = 0; channel < kAnalogChannels; channel++)
		{
			dashboardPacker.AddFloat(m_AnalogChannels[module][channel]);
		}
		dashboardPacker.FinalizeCluster();
	}
	// Pack the digital modules
	for (module = 0; module < kDigitalModules; module++)
	{
		dashboardPacker.AddCluster();
		dashboardPacker.AddU8(m_RelayFwd[module]);
		dashboardPacker.AddU8(m_RelayRev[module]);
		dashboardPacker.AddU16(m_DIOChannels[module]);
		dashboardPacker.AddU16(m_DIOChannelsOutputEnable[module]);
		dashboardPacker.AddCluster();
		for(channel = 0; channel < kPwmChannels; channel++)
		{
			dashboardPacker.AddU8(m_PWMChannels[module][channel]);
		}
		dashboardPacker.FinalizeCluster();
		dashboardPacker.FinalizeCluster();
	}
	// Pack the solenoid module
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
	
	dashboardPacker.AddFloat(m_accelX);
	dashboardPacker.AddFloat(m_accelY);
	dashboardPacker.AddFloat(m_accelZ);
	
	// Add target data

	// Add color target code
	
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
	
	dashboardPacker.AddU16(this->m_LeftState);
	dashboardPacker.AddU16(this->m_RightState);
	dashboardPacker.AddU16(this->m_TurretState);
	dashboardPacker.AddFloat(m_TurretPotentEncoderVoltage);
	
//	dashboardPacker.AddI32(m_LeftFollowerWheel);
//	dashboardPacker.AddI32(m_RightFollowerWheel);
	// Flush the data to the driver station.
	dashboardPacker.Finalize();
}
