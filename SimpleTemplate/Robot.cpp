//#include "Robot.h"
#include "CameraControl.h"
#include "WPILib.h"
#include "DashboardDataFormat.h"

#include <climits>
#include <cmath>
using std::cout;
using std::endl;
using ::floor;
// #include "vxWorks.h"
// #include "AxisCamera.h"
// #include "BaeUtilities.h"
// #include "FrcError.h"
// #include "PCVideoServer.h"
#include "Autonomous.h"
#include "Logger.h"


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
	Victor testMotor;
	int counter;
	// bool printed;
	Autonomous Auto;
	
public:
	PurpleMonkeys(void) :
		myRobot(1, 2), // these must be initialized in the same order
				stick(1), // as they are declared above.
				test(2),
				light(4, 14),
				// logger(),
		  		theCamera(),
				dashboardDataFormat(),
				testEncoder(4,1,4,2,false),
				testMotor(4,3),
				counter(0),
				//printed(false)
				Auto()
				{
		// GetTrackingData(YELLOW, PASSIVE_LIGHT);
		 
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
		 // logger.OpenFile("log.log");
		 // logger.Debug("Entering teleop mode...");
		 // logger.CloseFile();
		 
		light.Pulse(0.5);
		Auto.Init();
	}

	void AutonomousPeriodic(void) {
		UpdateDashboard();
		// static int counter = 0;
		counter++;
		if (counter>=400)
			myRobot.Drive(0.0, 0.0);
		Auto.Periodic();
	}

	void AutonomousContinuous(void) {
		// UpdateDashboard();
		Auto.Continuous();
	}

	// Teleop state methods
	void TeleopInit(void) {
		// myRobot.Drive(0.0, 0.0); // stop robot
		// myRobot.Drive(0.5, 0.0); // Go Straight Forward!
		GetWatchdog().SetEnabled(true);
	}

	void TeleopPeriodic(void) {

		GetWatchdog().Feed();
		
		myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
		
		// testMotor.SetRaw((UINT8)floor(test.GetRawAxis( Joystick::kDefaultYAxis )));
		testMotor.Set(test.GetY());
		
		UpdateDashboard();
	}

	void TeleopContinuous(void) {
	}
	
	 void UpdateDashboard(void)
	 {
	 static float num = 0.0;
//	 INT32 count = testEncoder.Get();
	 
	 //INT8 lower = count & 0xFF;
	 //dashboardDataFormat.m_AnalogChannels[0][4] = static_cast<float>(count); // / static_cast<float>(INT_MAX); 
	 // dashboardDataFormat.m_AnalogChannels[0][1] = 5.0 - num;
	 dashboardDataFormat.m_AnalogChannels[0][0] = stick.GetY(); 
	 dashboardDataFormat.m_AnalogChannels[0][1] = test.GetY();
	 dashboardDataFormat.m_AnalogChannels[0][4] = testMotor.Get();
	 dashboardDataFormat.m_DIOChannels[0]++;
	 dashboardDataFormat.m_DIOChannelsOutputEnable[0]--;
	 dashboardDataFormat.m_testEncoder = testEncoder.Get();
	 
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

