// #include "Autonomous.h"
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
	// Autonomous Auto;
	
public:
	PurpleMonkeys(void);

	// StartCompetition loop
	//void StartCompetition(void)
	//{
	//}

	// One time initialization of the robot
	void RobotInit(void);
	void DisabledInit(void);

	void DisabledPeriodic(void);

	void DisabledContinuous(void);
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
