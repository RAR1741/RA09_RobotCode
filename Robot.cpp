#include "WPILib.h"
#include <nivision.h>
#include "CameraControl.h"
#include "DashboardDataFormat.h"
#include "vxWorks.h"
#include "AxisCamera.h"
#include "BaeUtilities.h"
#include "FrcError.h"
#include "PCVideoServer.h"
#include "Logger.h"

using std::cout;
using std::endl;


/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */



class PurpleMonkeys : public IterativeRobot {
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	bool printed;
	DigitalOutput light;
	//Logger logger;
	CameraControl theCamera;
	DashboardDataFormat  dashboardDataFormat;
public:
	PurpleMonkeys(void) :
		myRobot(1, 2), // these must be initialized in the same order
				stick(1), // as they are declared above.
				printed(false),
				light(4,14),
	//			logger(),
				theCamera()
//				dashboardDataFormat()
				{
		
		if(StartCameraTask(13,0,k320x240,ROT_0)==-1)
		{
			dprintf("Things screwed up with camera.\n");
		}
		//StartCameraTask(13,0,k320x240,ROT_0);
		
		Wait(2.0);
		theCamera.Start();
	}

	// StartCompetition loop
	//void StartCompetition(void)
	//{
	//}

	// One time initialization of the robot
	void RobotInit(void) {
		GetWatchdog().SetExpiration(100);
	}

	// Disabled state methods
	void DisabledInit(void) {

	}

	void DisabledPeriodic(void) {

	}

	void DisabledContinuous(void) {
	}

	// Autonomous state methods
	void AutonomousInit(void) {
		//myRobot.Drive(0.5, 0.0); 	// drive forwards half speed
/*
		GetWatchdog().SetEnabled(false);
		logger.OpenFile("log.log");
		logger.Debug("Entering teleop mode...");
		logger.CloseFile();
*/
		light.Pulse(0.5);
	}

	void AutonomousPeriodic(void) {
		UpdateDashboard();
	}

	void AutonomousContinuous(void) {
		//UpdateDashboard();
	}

	// Teleop state methods
	void TeleopInit(void) {
		//myRobot.Drive(0.0, 0.0); // stop robot
		myRobot.Drive(0.5, 0.0);	// Go Straight Forward!
		GetWatchdog().SetEnabled(true);
	}

	void TeleopPeriodic(void) {

		GetWatchdog().Feed();
		UpdateDashboard();
		myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
	}

	void TeleopContinuous(void) {
	}
	void UpdateDashboard(void)
	{
		static float num = 0.0;
		dashboardDataFormat.m_AnalogChannels[0][0] = num;
		dashboardDataFormat.m_AnalogChannels[0][1] = 5.0 - num;
		dashboardDataFormat.m_DIOChannels[0]++;
		dashboardDataFormat.m_DIOChannelsOutputEnable[0]--;
		num += 0.01;
		if (num > 5.0) num = 0.0;
		dashboardDataFormat.PackAndSend();
	}
	
	Dashboard& GetTheDashboard()
	{
		return m_ds->GetDashboardPacker();
	}
};

START_ROBOT_CLASS(PurpleMonkeys);

