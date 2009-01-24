#include "WPILib.h"
// #include <nivision.h>

#include "CameraControl.h"
#include "DashboardDataFormat.h"
// #include "vxWorks.h"
// #include "AxisCamera.h"
// #include "BaeUtilities.h"
// #include "FrcError.h"
// #include "PCVideoServer.h"
#include "Logger.h"
#include <climits>

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
	Joystick test;
	DigitalOutput light;
 	// Logger logger;
	CameraControl theCamera;
	DashboardDataFormat dashboardDataFormat;
	Encoder testEncoder;
	// Pwm testMotor;
	int counter;
	bool printed;
	
public:
	PurpleMonkeys(void) :
		myRobot(1, 2), // these must be initialized in the same order
				stick(1), // as they are declared above.
				test(2),
				light(4, 14),
				// logger(),
				theCamera(),
				dashboardDataFormat(),
				testEncoder(1,2,false)	 // A: port 1 B: port2 Reversed: False
				// testMotor(3)
				{

		 //printed = false;
	}

	// StartCompetition loop
	//void StartCompetition(void)
	//{
	//}

	// One time initialization of the robot
	void RobotInit(void) {
		GetWatchdog().SetExpiration(100);

		
		 if(StartCameraTask(13,0,k320x240,ROT_0)==-1)
		 {
		 dprintf("Things screwed up with camera.\n");
		 }
		 
		 theCamera.Stop();
		 Wait(2.0);
		 theCamera.Start();
		 
		 dprintf("Robot initialized.");
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
		myRobot.Drive(0.5, 0.0); // drive forwards half speed
		GetWatchdog().SetEnabled(false);
		 counter = 0;
		 // logger.OpenFile("log.log");
		 // logger.Debug("Entering teleop mode...");
		 // logger.CloseFile();
		 
		light.Pulse(0.5);
	}

	void AutonomousPeriodic(void) {
		UpdateDashboard();
		// static int counter = 0;
		counter++;
		if (counter>=400)
			myRobot.Drive(0.0, 0.0);
	}

	void AutonomousContinuous(void) {
		// UpdateDashboard();
	}

	// Teleop state methods
	void TeleopInit(void) {
		// myRobot.Drive(0.0, 0.0); // stop robot
		myRobot.Drive(0.5, 0.0); // Go Straight Forward!
		GetWatchdog().SetEnabled(true);
	}

	void TeleopPeriodic(void) {

		GetWatchdog().Feed();
		UpdateDashboard();
		myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
		
		// testMotor.setRaw(test.getAxis(AxisType))
	}

	void TeleopContinuous(void) {
	}
	
	 void UpdateDashboard(void)
	 {
	 static float num = 0.0;
	 // INT32 count = testEncoder.Get();
	 
	 //INT8 lower = count & 0xFF;
	 // dashboardDataFormat.m_AnalogChannels[0][0] = static_cast<float>(count) / static_cast<float>(INT_MAX); 
	 dashboardDataFormat.m_AnalogChannels[0][1] = 5.0 - num;
	 dashboardDataFormat.m_DIOChannels[0]++;
	 dashboardDataFormat.m_DIOChannelsOutputEnable[0]--;
	 num += 0.01;
	 if (num > 5.0) num = 0.0;
	 dashboardDataFormat.PackAndSend();
	 }

	Dashboard& GetTheDashboard() {
		return m_ds->GetDashboardPacker();
	}
};

START_ROBOT_CLASS(PurpleMonkeys)
;

