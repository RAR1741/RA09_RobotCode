// #include "robot.h"
#ifndef PURPLE_MONKEYS_CPP__
#define PURPLE_MONKEYS_CPP__

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

#include "WPILib.h"
#include "Personality.h"
#include "Turret.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */

class PurpleMonkeys : public IterativeRobot {
	RobotDrive myRobot; // robot drive system
	//Joystick stick; // only joystick
	//Joystick test;
	Joystick leftStick;
	Joystick rightStick;
	Joystick turretStick;
	Joystick testMotorStick; // For the Test Motor Encoder
	DigitalOutput light;
	// Logger logger;
	CameraControl theCamera;
	DashboardDataFormat dashboardDataFormat;
	Encoder testEncoder;
	Victor testMotor;
	Servo horizontalServo;
	Servo verticalServo;
	int counter;
	Gyro * testGyro;
	Gyro * testTemp;
	// bool printed;
	// Autonomous Auto;
	Personality * Squeeky;
	Turret TheTurret;
	AnalogChannel leftCurrent;
	Accelerometer testAccel_X, testAccel_Y, testAccel_Z;

	enum // Driver Station jumpers to control program operation
	{	ARCADE_MODE = 1, // Tank/Arcade jumper is on DS Input 1 (Jumper present is arcade)
		ENABLE_AUTONOMOUS = 2, // Autonomous/Teleop jumper is on DS Input 2 (Jumper present is autonomous)
	} jumpers;

public:
	PurpleMonkeys(void) :
				myRobot(1, 2), // these must be initialized in the same order
				//stick(1), // as they are declared above.
				//test(2),
				leftStick(1), rightStick(2), turretStick(3), testMotorStick(4),

				light(4, 14),
				// logger(),
				theCamera(), dashboardDataFormat(), testEncoder(4, 1, 4, 2,
						false), testMotor(4, 3), horizontalServo(4, 4),
				verticalServo(4, 5),

				counter(0), 
				testGyro(NULL),
				testTemp(NULL),
				
				
				//printed(false)

				//Auto()
				Squeeky(NULL), TheTurret(), leftCurrent(2, 1),
				testAccel_X(1, 3), testAccel_Y(1, 4), testAccel_Z(1, 5) {
		// GetTrackingData(YELLOW, PASSIVE_LIGHT);

	}

	// StartCompetition loop
	//void StartCompetition(void)
	//{
	//}


	// One time initialization of the robot
	void RobotInit(void) {
		GetWatchdog().SetExpiration(100);

		if (testGyro==NULL)
			testGyro = new Gyro(1,1);
		//if (testTemp==NULL)
		// 	testTemp = new Gyro(1,2);
		if (StartCameraTask(13, 0, k320x240, ROT_0)==-1) {
			dprintf("Things screwed up with camera.\n");
		}

		dprintf("Robot initialized.");
	}

	// Disabled state methods
	void DisabledInit(void) {
		Squeeky = new Personality();
		if (testGyro==NULL)
			testGyro = new Gyro(1,1);

		UpdateDashboard();

	}

	void DisabledPeriodic(void) {
		Squeeky->RPTCommandProccessor();
	}

	void DisabledContinuous(void) {
	}

	// Autonomous state methods
	void AutonomousInit(void) {
		testEncoder.Stop();
		testMotor.Set(0.0);
		//myRobot.Drive(0.5, 0.0); // drive forwards half speed
		GetWatchdog().SetEnabled(false);
		// logger.OpenFile("log.log");
		// logger.Debug("Entering teleop mode...");
		// logger.CloseFile();

		light.Set(0);
		// Auto.Init();
	}

	// This function is passed a normalized x coordinate value
	// of a target (e.g. an enemy trailer) acquired from the camera via the Turret class
	// Patrick wrote. It's supposed to determine how to turn the robot
	// to get the target approximatly centered, and then drive after it.
	// Will need to implement it at later date, and probably reorganize the
	// code into the Autonomous class.
	void FetchBoy(double x_val) {
		// Decide How to turn
	}

	void AutonomousPeriodic(void) {
		UpdateDashboard();
		// static int counter = 0;
		// counter++;
		// if (counter>=400)
		//	myRobot.Drive(0.0, 0.0);
		// Auto.Periodic();
		TheTurret.TurretControl(); // This updates the Target in Sight member variable;

		if (TheTurret.TargetInSight()) {
			GetTheDashboard().Printf("TARGET IN SIGHT!");
			FetchBoy(TheTurret.GetTarget_X());
		} else {
			GetTheDashboard().Printf("WHERE?!");
			myRobot.Drive(0.0, 0.0);
		}

	}

	void AutonomousContinuous(void) {
		// UpdateDashboard();
		// Auto.Continuous();
	}

	// Teleop state methods
	void TeleopInit(void) {
		testEncoder.SetDistancePerPulse(300.0);
		testEncoder.Start();
		if (testGyro==NULL)
			testGyro = new Gyro(1,1);
		light.Set(1);
		// myRobot.Drive(0.0, 0.0); // stop robot
		// myRobot.Drive(0.5, 0.0); // Go Straight Forward!
		GetWatchdog().SetEnabled(true);
	}

	void TeleopPeriodic(void) {

		GetWatchdog().Feed();

		//myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)

		// testMotor.SetRaw((UINT8)floor(test.GetRawAxis( Joystick::kDefaultYAxis )));
		//testMotor.Set(test.GetY());
		// determine if tank or arcade mode; default with no jumper is for tank drive
		if (m_ds->GetDigitalIn(ARCADE_MODE) == 0) {
			// myRobot.TankDrive(leftStick, rightStick);	 // drive with tank style
			myRobot.TankDrive(leftStick.GetY(), rightStick.GetY());
		} else {
			myRobot.ArcadeDrive(rightStick.GetY(), -rightStick.GetX()); // drive with arcade style (use right stick)
		}

		// Just for testing, comment if you don't want it.
		testMotor.Set(testMotorStick.GetY()); // Set Test Motor based on Y Axis

		if (testMotorStick.GetButton(testMotorStick.kTopButton)) {
			testEncoder.Reset();
			delete testGyro;
			testGyro = new Gyro(1,1);
			// delete testTemp;
			// testTemp = new Gyro(1,2);
		}
		
		if (leftStick.GetButton(leftStick.kTriggerButton)) {
			GetTheDashboard().Printf("TRIGGA!");
			if (Squeeky == NULL) {
				// Uh oh! 
				testMotor.Set(0.999);
			} else {
				Squeeky->SqueekySayHello();
				testMotor.Set(0.999);
			}
		} else {
			testMotor.Set(0.0);
		}

		// horizontalServo.Set((turretStick.GetX()+ 1.0) / 2.0);
		// verticalServo.Set((turretStick.GetY()+ 1.0) / 2.0);
		UpdateDashboard();
	}

	void TeleopContinuous(void) {
	}

	void UpdateDashboard(void) {
		static float num = 0.0;
		//	 INT32 count = testEncoder.Get();
		// INT32 encoder_raw; = testEncoder.Get();
		//INT8 lower = count & 0xFF;
		//dashboardDataFormat.m_AnalogChannels[0][4] = static_cast<float>(count); // / static_cast<float>(INT_MAX); 
		// dashboardDataFormat.m_AnalogChannels[0][1] = 5.0 - num;
		//dashboardDataFormat.m_AnalogChannels[0][0] = stick.GetY(); 
		// dashboardDataFormat.m_AnalogChannels[0][1] = test.GetY();
		dashboardDataFormat.m_AnalogChannels[0][4] = testMotor.Get();
		dashboardDataFormat.m_DIOChannels[0]++;
		dashboardDataFormat.m_DIOChannelsOutputEnable[0]--;
		dashboardDataFormat.m_testEncoder = testEncoder.Get();
		dashboardDataFormat.m_LeftMotorVoltage = leftCurrent.GetValue();
		dashboardDataFormat.m_accelX = testAccel_X.GetAcceleration();
		dashboardDataFormat.m_accelY = testAccel_Y.GetAcceleration();
		dashboardDataFormat.m_accelZ = testAccel_Z.GetAcceleration();
		if (testGyro == NULL) {
			dashboardDataFormat.m_gyroAngle = -42.0001;
		} else {
			dashboardDataFormat.m_gyroAngle = testGyro->GetAngle();
			//dashboardDataFormat.m_gyroAngle = 589.7;
		}
	// 	dashboardDataFormat.m_gyroTemp = testTemp->GetAngle(); // Added 2/6/2009 HAM

		//  dashboardDataFormat.m_accelX = 84.0;
		/*
		GetTheDashboard().Printf(
				"Encoder Counts: %d, Distance: %f, Gyro Angle: %f, Left Motor Voltage: %d",
				dashboardDataFormat.m_testEncoder, testEncoder.GetDistance(),
				testGyro->GetAngle(), dashboardDataFormat.m_LeftMotorVoltage);
		*/
		num += 0.01;
		if (num > 5.0)
			num = 0.0;
		dashboardDataFormat.PackAndSend();
	}

	Dashboard& GetTheDashboard() {
		return m_ds->GetDashboardPacker();
	}

	void SeekTarget() {
		/*
		 if ( FindTwoColors(td1, td2, ABOVE, &par) ){
		 found = true;
		 } else {
		 found = false;
		 }
		 */
	}

};

START_ROBOT_CLASS(PurpleMonkeys)
;
#endif
