
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
	dashboardPacker.AddU32(m_RightBoundRect);
	dashboardPacker.AddU32(m_BottomBoundRect);
	// PJF RED ALERT CODE: Add encoder data
	
	dashboardPacker.AddI32(m_LM_QuadEncoder);
	dashboardPacker.AddI32(m_RM_QuadEncoder);
	dashboardPacker.AddI32(m_LMWheelEncoder);
	dashboardPacker.AddI32(m_RMWheelEncoder);
	
	dashboardPacker.AddFloat(m_gyroAngle);
	dashboardPacker.AddFloat(m_gyroTemp);    // Added by HAM 2/6/9
	
	dashboardPacker.AddFloat(m_accelX);
	dashboardPacker.AddFloat(m_accelY);
	dashboardPacker.AddFloat(m_accelZ);
	
	// Add target data

		
		
	// Flush the data to the driver station.
	dashboardPacker.Finalize();
}
