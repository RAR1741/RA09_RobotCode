#include "WPILib.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class PurpleMonkeys : public IterativeRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick

public:
	PurpleMonkeys(void):
		myRobot(1, 2),	// these must be initialized in the same order
		stick(1)		// as they are declared above.
	{
	}

	// StartCompetition loop
	//void StartCompetition(void)
	//{
	//}

	// One time initialization of the robot
	void RobotInit(void)
	{
		GetWatchdog().SetExpiration(100);
	}

	// Disabled state methods
	void DisabledInit(void)
	{
	}

	void DisabledPeriodic(void)
	{
	}

	void DisabledContinuous(void)
	{
	}

	// Autonomous state methods
	void AutonomousInit(void)
	{
		GetWatchdog().SetEnabled(false);
	}

	void AutonomousPeriodic(void)
	{
		myRobot.Drive(0.5, 0.0); 	// drive forwards half speed
	}

	void AutonomousContinuous(void)
	{
	}

	// Teleop state methods
	void TeleopInit(void)
	{
		myRobot.Drive(0.0, 0.0); 	// stop robot
		GetWatchdog().SetEnabled(true);
	}

	void TeleopPeriodic(void)
	{
		GetWatchdog().Feed();
		myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
	}

	void TeleopContinuous(void)
	{
	}
	
};

START_ROBOT_CLASS(PurpleMonkeys);

