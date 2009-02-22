// #include "robot.h"
#ifndef PURPLE_MONKEYS_CPP__
#define PURPLE_MONKEYS_CPP__

#include "CameraControl.h"
#include "WPILib.h"
#include "DashboardDataFormat.h"
#include "DriverStationLCD.h"

#include <climits>
#include <cmath>
using std::cout;
using std::endl;
using ::floor;
#include "Autonomous.h"
#include "Logger.h"

#include "WPILib.h"
#include "Personality.h"
#include "Turret.h"
#include "Toggle.h"
#include "Harvester.h"
#include "Elevator.h"
#include "Launcher.h"
#include "EmptyCellCatcher.h"
#include "GateController.h"
#include "Mode.h"
#include "DriverStationLCD.h"

#define USR_RQ_EJECT 0
#define USR_RQ_IDLE 1

#define USR_RQ_RUN true
#define USR_RQ_STOP false

/*
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */

class PurpleMonkeys : public IterativeRobot {
	RobotDrive myRobot; // robot drive system
	Joystick leftStick;
	Joystick rightStick;
	Joystick turretStick;
	Joystick testMotorStick; // For the Test Motor Encoder
	DigitalOutput light;
	// Logger logger;
	CameraControl theCamera;
	DashboardDataFormat dashboardDataFormat;
	Encoder LMQuadEncoder;
	Encoder RMQuadEncoder;
	Encoder LMWheelEncoder;
	Encoder RMWheelEncoder;
	Encoder TurretEncoder;

	int counter;
	Gyro * testGyro;
	Gyro * testTemp;
	Personality * Squeeky;
	Turret TheTurret;
	AnalogChannel leftCurrent;
	AnalogChannel rightCurrent;

	AnalogChannel turretPositionCurrent;

	Accelerometer testAccel_X, testAccel_Y, testAccel_Z;
	//Solenoid Gate;
	RobotHarvester Harvester;
	RobotElevator Elevator;
	GateController TheGate;
	CellCatcher ECellCatcher;
	// Solenoid Collector;
	Toggle Trigger;
	Toggle GateToggle;
	Toggle CellCatcherToggle;
	Toggle RunStopToggle;
	Launcher launcher;
	Compressor TheCompressor; // The air compressor for the robot.
	Encoder RFollowWheelEncoder;
	Encoder LFollowWheelEncoder;
	IFF *IdentFriendFoe;
	// State Variables for toggle code.

	// Elevator state vars
	unsigned int UserRequestElevatorArmDisarm;
	unsigned int UserRequestElevatorRunStop;
	unsigned int UserRequestElevatorFire;
	enum // Driver Station jumpers to control program operation
	{	ARCADE_MODE = 1, // Tank/Arcade jumper is on DS Input 1 (Jumper present is arcade)
		ENABLE_AUTONOMOUS = 2, // Autonomous/Teleop jumper is on DS Input 2 (Jumper present is autonomous)
		MODE_SWITCH_1 = 3,
		MODE_SWITCH_2 = 4,
	} jumpers;

	// 
	int MasterControlMode;
	bool AutoModeRunStop;
public:
	PurpleMonkeys(void) :
				myRobot(4, 5), // these must be initialized in the same order
				leftStick(1), 
				rightStick(2), 
				turretStick(3), 
				testMotorStick(4),

				light(4, 14), 
				theCamera(),
				dashboardDataFormat(),
				LMQuadEncoder(4, 1, 4, 2, false),
				RMQuadEncoder(4, 3, 4, 4,false),
				LMWheelEncoder(4, 5, 4, 6, false),
				RMWheelEncoder(4, 7, 4, 8, false),
				//ElevatorEncoder(4,11,4,12,false),
				TurretEncoder(4, 13, 4, 14, false),

				counter(0),
				testGyro(NULL), 
				testTemp(NULL), 
				Squeeky(NULL),
				TheTurret(), 
				leftCurrent(2, 4), 
				rightCurrent(2, 5),
				turretPositionCurrent(2, 3),
				testAccel_X(1, 3), 
				testAccel_Y(1, 4),
				testAccel_Z(1, 5),
				//				Gate(8,1),
				Harvester(4, 6, 2, 6), 
				Elevator(),
				// Collector(8,1),
				TheGate(),
				ECellCatcher(), 
				Trigger(&leftStick, 2), 
				GateToggle(&turretStick, 2), 
				CellCatcherToggle(&rightStick, 1),
				RunStopToggle(&turretStick, 11),
				launcher(),
				// No furthur control is really necessary.
				// Pressure switch, slot 6 channel 11
				// Spike relay, slot 4 channel 1
				TheCompressor(6, 11, 4, 1), 
				RFollowWheelEncoder(4, 7, 4, 8,false),
				LFollowWheelEncoder(4, 5, 4, 6, false)
				{
		// GetTrackingData(YELLOW, PASSIVE_LIGHT);

		MasterControlMode = 0; // Manual mode

	}
	// One time initialization of the robot
	void RobotInit(void) {
		MasterControlMode = MODE_MANUAL;
		GetWatchdog().SetExpiration(100);
		launcher.Init(4, 2);
		launcher.SetRun(true);
		launcher.SetJoyStick(&turretStick);

		
		AutoModeRunStop = false;
		if (testGyro==NULL)
			testGyro = new Gyro(1,1);
		if (testTemp==NULL)
			testTemp = new Gyro(1,2);
		if (StartCameraTask(13, 0, k320x240, ROT_0)==-1) {
			dprintf("Things screwed up with camera.\n");
		}

		if (theCamera.StatusIsFatal()) {
			dprintf("PCVideoServer is mortally wounded.\n");
		}
		
		dprintf("Initializing IFF module...");
		IdentFriendFoe = new IFF();
		dprintf("IFF complete.");
		// This should probably be in the Elevator code

		//if(!Gate.Get()) // Is the gate open?
		//	Gate.Set(false); // Yes? Then close it.

		Harvester.Init();
		Harvester.SetCollectEjectControls(&rightStick, 4);
		Harvester.SetRunStopControls(&rightStick, 3);
		Harvester.SetGateControls(&TheGate);
		Harvester.SetAutoMode(MasterControlMode);
		//Harvester.SetAutoMode(MODE_AUTO);

		// Initiallize the Elevator class
		Elevator.Init(4, 1, 2, 1, &Harvester);
		//JDM: Set the joystick and button to use to test the elevator
		Elevator.SetElevatorControls(&turretStick, 1);
		Elevator.SetAutoMode(MasterControlMode);

		TheTurret.InitTurret(4,3,	// Turret jaguar
							6,9,	// CW Limit Switch
							6,10,	// CCW Limit Switch
							1,7,	// Potentiometer Encoder
							IdentFriendFoe,	// IFF
							8);		// DS IO pin for Goggle
		ECellCatcher.Init(8, 3, 8, 4);
		Squeeky = new Personality();

		TheCompressor.Start();
		// LaunchEncoder.Start();
		TurretEncoder.Start();
		RMQuadEncoder.Start();
		LMQuadEncoder.Start();
		RMWheelEncoder.Start();
		LMWheelEncoder.Start();
		RFollowWheelEncoder.Start();
		LFollowWheelEncoder.Start();
		
		dprintf("Robot initialized.");
	}

	// Disabled state methods
	void DisabledInit(void) {
		MasterControlMode = MODE_MANUAL;
		Squeeky->Open();
		Squeeky->SqueekySayHello();
		if (testGyro==NULL)
			testGyro = new Gyro(1,1);

		UpdateDashboard();

	}

	void DisabledPeriodic(void) {
		CheckMode();
//		int DSMode;
//		DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
//
//		DSMode = m_ds->GetDigitalIn(MODE_SWITCH_1) * 2 + m_ds->GetDigitalIn(MODE_SWITCH_2);
//		switch (DSMode) {
//		case 0: //Manual mode
//			MasterControlMode = MODE_MANUAL;
//			break;
//		case 1: //Semi automatic
//			MasterControlMode = MODE_SEMI_AUTO;
//			break;
//		case 2: //Full automatic
//			//MasterControlMode = MODE_AUTO;
//			break;
//		default:
//			MasterControlMode = MODE_MANUAL;
//		}
//
//		switch (DSMode) {
//		case MODE_MANUAL:
//			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Manual        ");
//			break;
//		case MODE_SEMI_AUTO:
//			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Semi-Automatic");
//			break;
//		case MODE_AUTO:
//			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Full-Automatic");
//			break;
//		default:
//			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Manual (Error)");
//		}
		//Squeeky->RPTCommandProccessor();
	}

	void DisabledContinuous(void) {
	}

	// Autonomous state methods
	void AutonomousInit(void) {
		Squeeky->Close();
		LMQuadEncoder.Stop();
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

		// static int counter = 0;
		// counter++;
		// if (counter>=400)
		//	myRobot.Drive(0.0, 0.0);
		// Auto.Periodic();
		TheTurret.TurretControl(); // This updates the Target in Sight member variable;

		if (TheTurret.TargetInSight()) {
			//GetTheDashboard().Printf("TARGET IN SIGHT!");
			FetchBoy(TheTurret.GetTarget_X());
		} else {
			//GetTheDashboard().Printf("WHERE?!");
			myRobot.Drive(0.0, 0.0);
		}
		UpdateDashboard();
	}

	void AutonomousContinuous(void) {
		// UpdateDashboard();
		// Auto.Continuous();
	}

	// Teleop state methods
	void TeleopInit(void) {
		
		//		testEncoder.SetDistancePerTick(300.0);
		//		LMQuadEncoder.SetDistancePerPulse(300.0);
		LMQuadEncoder.Start();
		if (testGyro==NULL)
			testGyro = new Gyro(1,1);
		light.Set(1);
		// myRobot.Drive(0.0, 0.0); // stop robot
		// myRobot.Drive(0.5, 0.0); // Go Straight Forward!

		Harvester.SetAutoMode(MasterControlMode);
		Elevator.SetAutoMode(MasterControlMode);
		TheTurret.SetMode(MasterControlMode);
//		TheTurret.SetMode(MODE_MANUAL);
//		//TheTurret.RegisterMasterControl(&MasterControlMode);

		GetWatchdog().SetEnabled(true);
//#warning "The Watchdog NEEDS to be fed."
		
		//GetWatchdog().SetEnabled(false);
		// Launch_Wheels_Motor.Set(0.0);
	}

	bool CheckMode(void)
	{
		int DSMode = 0;
		bool ModeChanged;
		DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();

		DSMode = m_ds->GetDigitalIn(MODE_SWITCH_1) * 2 + m_ds->GetDigitalIn(MODE_SWITCH_2);
		ModeChanged = (DSMode == MasterControlMode);
		
		switch (DSMode) {
		case 3: //Manual mode
			MasterControlMode = MODE_MANUAL;
			break;
		case 1: //Semi automatic
			MasterControlMode = MODE_SEMI_AUTO;
			break;
		case 2: //Full automatic
			MasterControlMode = MODE_AUTO;
			break;
		default:
			MasterControlMode = MODE_MANUAL;
		}

		switch (DSMode) {
		case MODE_MANUAL:
			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Manual        ");
			break;
		case MODE_SEMI_AUTO:
			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Semi-Automatic");
			break;
		case MODE_AUTO:
			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Full-Automatic");
			break;
		default:
			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Manual (Error)");
		}
		dsLCD->UpdateLCD();
		return ModeChanged;
	}
	
	void TeleopPeriodic(void) {

		GetWatchdog().Feed();

		//DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();

		//myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)

		// testMotor.SetRaw((UINT8)floor(test.GetRawAxis( Joystick::kDefaultYAxis )));
		//testMotor.Set(test.GetY());
		// determine if tank or arcade mode; default with no jumper is for tank drive
		if (m_ds->GetDigitalIn(ARCADE_MODE) == 1) {
			// myRobot.TankDrive(leftStick, rightStick);	 // drive with tank style

			// This is a quick temporary for the inverted Y axis Mr. Meyer,
			// but I think the issue with the inverted joysticks is with the motors...
			// they may need to have the wires on their polarity switched.
			myRobot.TankDrive(-leftStick.GetY(), -rightStick.GetY());
		} else {
			myRobot.ArcadeDrive(-rightStick.GetY(), -rightStick.GetX()); // drive with arcade style (use right stick)
		}

		// Just for testing, comment if you don't want it.
		//testMotor.Set(testMotorStick.GetY()); // Set Test Motor based on Y Axis


		RunStopToggle.UpdateState();
		if(RunStopToggle.GetOutput())
			AutoModeRunStop = true;
		else
			AutoModeRunStop = false;
		
		if (testMotorStick.GetButton(testMotorStick.kTopButton)) {
			LMQuadEncoder.Reset();
			delete testGyro;
			testGyro = new Gyro(1,1);
		}

//		// Check to see if the Master Control Mode has changed
//		// If so, update the modes for the Harvester, Elevator, and Turret
//		if (CheckMode())
//		{
//			Harvester.SetAutoMode(MasterControlMode);
//			Elevator.SetAutoMode(MasterControlMode);
//			TheTurret.SetMode(MasterControlMode);
//		}

		//TheTurret.SetMode(MODE_)
		Elevator.Process(launcher.GetStatus(), AutoModeRunStop);
		// JDM: Use joystick to test, needs to use Elevator Load flag
		
		Harvester.Process(Elevator.GetHarvesterLoad(), AutoModeRunStop);
		launcher.Update();

		if(MasterControlMode == MODE_MANUAL)
			TheGate.Set(GateToggle.GetOutput());

		DriverStationLCD * dlcd = DriverStationLCD::GetInstance();
		dlcd->Printf(DriverStationLCD::kUser_Line4, 5, "%1d",
				GateToggle.GetOutput());

		ECellCatcher.Set(CellCatcherToggle.GetOutput());
		GateToggle.UpdateState();
		CellCatcherToggle.UpdateState();

		//#warning "THIS IS TEST CODE. DON'T SEND THE ROBOT INTO COMPETITION"

		// Map X-Axis of joystick to turret position
		//Turret_Pos_Motor.Set(turretStick.GetX());
		TheTurret.TurretControl(&turretStick);

		// Moved to elevator code.
		// Launch_Wheels_Motor.Set(-((-turretStick.GetZ()+1.0)/2.0));


		// horizontalServo.Set((turretStick.GetX()+ 1.0) / 2.0);
		// verticalServo.Set((turretStick.GetY()+ 1.0) / 2.0);

		AirCompressorControl(); // This doesn't really do anything
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

		if (!TheTurret.TargetInSight()) {
			dashboardDataFormat.m_TopBoundRect = 1;
			dashboardDataFormat.m_BottomBoundRect = 2;
			dashboardDataFormat.m_LeftBoundRect = 1;
			dashboardDataFormat.m_RightBoundRect = 2;
		} else {
			dashboardDataFormat.m_TopBoundRect = TheTurret.TargetData().boundingRect.top;
			dashboardDataFormat.m_LeftBoundRect = TheTurret.TargetData().boundingRect.left;
			dashboardDataFormat.m_BottomBoundRect = TheTurret.TargetData().boundingRect.top + TheTurret.TargetData().boundingRect.height;
			dashboardDataFormat.m_RightBoundRect = TheTurret.TargetData().boundingRect.left + TheTurret.TargetData().boundingRect.width;
		}
		//dashboardDataFormat.m_AnalogChannels[0][4] = testMotor.Get();
		dashboardDataFormat.m_DIOChannels[0]++;
		dashboardDataFormat.m_DIOChannelsOutputEnable[0]--;
		dashboardDataFormat.m_RM_QuadEncoder = RMQuadEncoder.Get();
		dashboardDataFormat.m_LM_QuadEncoder = LMQuadEncoder.Get();
		dashboardDataFormat.m_RMWheelEncoder = RMWheelEncoder.Get();
		dashboardDataFormat.m_LMWheelEncoder = LMWheelEncoder.Get();
		dashboardDataFormat.m_LeftMotorVoltage = leftCurrent.GetValue();
		dashboardDataFormat.m_RightMotorVoltage = rightCurrent.GetValue();
		dashboardDataFormat.m_launchWheelsCurrent
				= (INT16)(launcher.GetCurrentVal());
		dashboardDataFormat.m_turretPositionCurrent
				= turretPositionCurrent.GetValue();

		dashboardDataFormat.m_LaunchEncoder = launcher.GetEncoderVal();
		dashboardDataFormat.m_TurretEncoder = TurretEncoder.Get();

		dashboardDataFormat.m_accelX = testAccel_X.GetAcceleration();
		dashboardDataFormat.m_accelY = testAccel_Y.GetAcceleration();
		dashboardDataFormat.m_accelZ = testAccel_Z.GetAcceleration();
		dashboardDataFormat.m_gyroTemp = testTemp->GetAngle();

		if (testGyro == NULL) {
			dashboardDataFormat.m_gyroAngle = -42.0001;
		} else {
			dashboardDataFormat.m_gyroAngle = testGyro->GetAngle();
			//dashboardDataFormat.m_gyroAngle = 589.7;
		}
		//  dashboardDataFormat.m_accelX = 84.0;

		dashboardDataFormat.m_TurretState = 0;
		dashboardDataFormat.m_LeftState = 0;
		dashboardDataFormat.m_RightState = 0;

		dashboardDataFormat.m_TurretPotentEncoderVoltage
				= TheTurret.EncoderVoltage();

		/*
		GetTheDashboard().Printf(
				"Encoder Counts: %d, Distance: %f, Gyro Angle: %f, Left Motor Voltage: %d",
				dashboardDataFormat.m_RM_QuadEncoder,
				LMQuadEncoder.GetDistance(), testGyro->GetAngle(),
				dashboardDataFormat.m_LeftMotorVoltage);
		*/
		GetTheDashboard().Printf("*");
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

	UINT16 PackJoystickState(Joystick *stick) {
		UINT16 retval = 0;
		for (int i = 0; i <= 12; i++) {
			retval |= (stick->GetRawButton(i) ? 1 : 0);
			retval <<= 1;
		}
		return retval;
	}

	void AirCompressorControl(void) {

	}

};

START_ROBOT_CLASS(PurpleMonkeys)
;
#endif

