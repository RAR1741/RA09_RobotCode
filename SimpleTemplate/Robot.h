
#include "CameraControl.h"
#include "WPILib.h"
#include "DashboardDataFormat.h"

#include <climits>
#include <cmath>
using std::cout;
using std::endl;
using ::floor;


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
				counter(0)//,
				//printed(false)
				{
		// GetTrackingData(YELLOW, PASSIVE_LIGHT);
		 
	}

	void RobotInit(void);
	// Disabled state methods
	void DisabledInit(void);

	void DisabledPeriodic(void);

	void DisabledContinuous(void) {
	}

	// Autonomous state methods
	void AutonomousInit(void);
	void AutonomousPeriodic(void);

	void AutonomousContinuous(void);
	// Teleop state methods
	void TeleopInit(void);

	void TeleopPeriodic(void);

	void TeleopContinuous(void);
	 void UpdateDashboard(void);

	Dashboard& GetTheDashboard();
};
