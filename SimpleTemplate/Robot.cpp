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
using ::fabs;

#include "Autonomous.h"
#include "Logger.h"

#include "WPILib.h"
#include "Personality.h"
#include "Turret.h"
#include "AutoTurret.h"
#include "Toggle.h"
#include "Harvester.h"
#include "Elevator.h"
#include "Launcher.h"
#include "EmptyCellCatcher.h"
#include "GateController.h"
#include "Mode.h"
#include "DriverStationLCD.h"
#include "CurrentSensor.h"
#include "RobotState.h"
#include "AccelerationLimiter.h"

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
	RobotDrive *myRobot; // robot drive system
	Joystick *leftStick;
	Joystick *rightStick;
	Joystick *turretStick;
	Joystick *testMotorStick; // For the Test Motor Encoder

	// Logger logger;
	CameraControl *theCamera;
	DashboardDataFormat *dashboardDataFormat;
	Encoder *LMQuadEncoder;
	Encoder *RMQuadEncoder;
	Encoder *LMWheelEncoder;
	Encoder *RMWheelEncoder;
	//Encoder *TurretEncoder;

	int counter;
	Gyro * testGyro;
	AnalogChannel * testTemp;
	Personality * Squeeky;
	Turret *TheTurret;
	//AnalogChannel *leftCurrent;
	//AnalogChannel *rightCurrent;
	
	CurrentSensor *leftCurrent;
	CurrentSensor *rightCurrent;

	AnalogChannel *turretPositionCurrent;

	Accelerometer *testAccel_X, *testAccel_Y, *testAccel_Z;
	//Solenoid Gate;
	Autonomous *AutoProgram;
	RobotHarvester *Harvester;
	RobotElevator *Elevator;
	GateController *TheGate;
	CellCatcher *ECellCatcher;
	// Solenoid Collector;
	Toggle *Trigger;
	Toggle *GateToggle;
	Toggle *CellCatcherToggle;
	Toggle *RunStopToggle;
	Launcher *launcher;
	Compressor *TheCompressor; // The air compressor for the robot.
	Encoder *RFollowWheelEncoder;
	Encoder *LFollowWheelEncoder;
	IFF *IdentFriendFoe;
	RobotState *r_state;
	
	AnalogChannel *ultrasonic;
	
	AccelerationLimiter *leftstick_limit;
	AccelerationLimiter *rightstick_limit;
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
		AUTONOMOUS_PROGRAM_SWITCH_1 = 7,
		AUTONOMOUS_PROGRAM_SWITCH_2 = 8,
	} jumpers;

	// 
	int MasterControlMode;
	UINT32 MasterProgramNumber;
	bool AutoModeRunStop;
	DriverStationLCD *dsLCD;
	UINT32 MasterOperatingMode; // This var is used to report where the robot code is executing to the logger.
public:
	PurpleMonkeys(void)
#if 0
	:
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
				AutoProgram(NULL),
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
#endif
				{
		// GetTrackingData(YELLOW, PASSIVE_LIGHT);
		myRobot = NULL;
		//myRobot(4, 5), // these must be initialized in the same order
		leftStick = NULL; //		leftStick(1), 
		rightStick = NULL; //		rightStick(2), 
		turretStick = NULL; //		turretStick(3), 
		testMotorStick = NULL; //		testMotorStick(4),

		//light = NULL; //		light(4, 14), 
		theCamera = NULL; //		theCamera(),
		dashboardDataFormat = NULL; //		dashboardDataFormat(),
		LMQuadEncoder = NULL;	//	LMQuadEncoder(4, 1, 4, 2, false),
		RMQuadEncoder = NULL; //		RMQuadEncoder(4, 3, 4, 4,false),
		LMWheelEncoder = NULL;  //		LMWheelEncoder(4, 5, 4, 6, false),
		RMWheelEncoder = NULL; //		RMWheelEncoder(4, 7, 4, 8, false),
				//ElevatorEncoder(4,11,4,12,false),
		//TurretEncoder = NULL;		// TurretEncoder(4, 13, 4, 14, false),

		counter = 0;
		//		counter(0),
		
		testGyro = NULL;
				//testGyro(NULL),
		
		testTemp = NULL;
				//testTemp(NULL),
		
		Squeeky = NULL;
				//Squeeky(NULL),
		
		TheTurret = NULL;
				//TheTurret(), 
		
		leftCurrent = NULL;
				//leftCurrent(2, 4), 
		
		rightCurrent = NULL;
				//rightCurrent(2, 5),
		
		turretPositionCurrent = NULL;
		
				//turretPositionCurrent(2, 3),
		
		testAccel_X = NULL;
		
		//		testAccel_X(1, 3), 
		testAccel_Y = NULL;	//		testAccel_Y(1, 4),
		testAccel_Z = NULL;	//		testAccel_Z(1, 5),
				//				Gate(8,1),
		AutoProgram = NULL;
		Harvester = NULL; //Harvester(4, 6, 2, 6), 
		Elevator = NULL;			//Elevator(),
		//Collector = NULL;		// Collector(8,1),
		TheGate = NULL;	//		TheGate(),
		ECellCatcher = NULL; //		ECellCatcher(), 
		Trigger = NULL;	//		Trigger(&leftStick, 2), 
		GateToggle = NULL; //		GateToggle(&turretStick, 2), 
		CellCatcherToggle = NULL; //		CellCatcherToggle(&rightStick, 1),
		RunStopToggle = NULL; //		RunStopToggle(&turretStick, 11),
		launcher = NULL;	//		launcher(),
				// No furthur control is really necessary.
				// Pressure switch, slot 6 channel 11
				// Spike relay, slot 4 channel 1
		TheCompressor = NULL;		//TheCompressor(6, 11, 4, 1), 
		RFollowWheelEncoder = NULL;	//		RFollowWheelEncoder(4, 7, 4, 8,false),
		LFollowWheelEncoder = NULL; 	//		LFollowWheelEncoder(4, 5, 4, 6, false)
		MasterControlMode = 0; // Manual mode
		MasterProgramNumber = 0;
		r_state = NULL;
		
		ultrasonic = NULL;
		
		leftstick_limit = NULL;
		rightstick_limit = NULL;
		SetDebugFlag(DEBUG_SCREEN_ONLY);
	}
	// One time initialization of the robot
	void RobotInit(void) {
		
		// GetTrackingData(YELLOW, PASSIVE_LIGHT);
		dprintf(LOG_INFO,"================================================");
		dprintf(LOG_INFO,"               INITIALIZING");
		dprintf(LOG_INFO,"================================================");
		dprintf(LOG_INFO,"RedAlert: Initializing Robot Drive");
				myRobot = new RobotDrive(4,5);
		dprintf(LOG_INFO,"RedAlert: Robot Drive Initialized.");
		dprintf(LOG_INFO,"RedAlert: Initializing Joysticks");
				//myRobot(4, 5), // these must be initialized in the same order
				leftStick = new Joystick(1); //		leftStick(1), 
				rightStick = new Joystick(2); //		rightStick(2), 
				turretStick = new Joystick(3); //		turretStick(3), 
				testMotorStick = new Joystick(4); //		testMotorStick(4),
		dprintf(LOG_INFO,"RedAlert: Joysticks Initialized.");
		dprintf(LOG_INFO,"RedAlert: Initializing Acceleration Control");
				leftstick_limit = new AccelerationLimiter(1.0 / this->GetLoopsPerSec(), 0.5);
				rightstick_limit = new AccelerationLimiter(1.0 / this->GetLoopsPerSec(), 0.5);
		dprintf(LOG_INFO,"RedAlert: AccelerationControl Initialized.");
		dprintf(LOG_INFO,"RedAlert: Initializing Autonomous Light");
				//light = new DigitalOutput(6,14); //		light(4, 14),
		dprintf(LOG_INFO,"RedAlert: Autonomous Light Initialized.");
		dprintf(LOG_INFO,"RedAlert: Initializing Camera Control");
				theCamera = new CameraControl(); //		theCamera(),
		dprintf(LOG_INFO,"RedAlert: Camera Control Initialized.");
		dprintf(LOG_INFO,"RedAlert: Initializing DashboardDataFormat");
				dashboardDataFormat = new DashboardDataFormat(); //		dashboardDataFormat(),
		dprintf(LOG_INFO,"RedAlert: DashboardDataFormat Initialized.");
		dprintf(LOG_INFO,"RedAlert: Initializing Drive Encoders");
				LMQuadEncoder = new Encoder(4, 1, 4, 2, false);	//	LMQuadEncoder(4, 1, 4, 2, false),
		//dprintf(LOG_INFO,"RedAlert: ");
				RMQuadEncoder = new Encoder(4, 3, 4, 4,false); //		RMQuadEncoder(4, 3, 4, 4,false),
				LMWheelEncoder = new Encoder(4, 5, 4, 6, true);  //		LMWheelEncoder(4, 5, 4, 6, false),
				RMWheelEncoder = new Encoder(4, 7, 4, 8, true); //		RMWheelEncoder(4, 7, 4, 8, false),
				

				LMWheelEncoder->Start();
				RMWheelEncoder->Start();
				
						//ElevatorEncoder(4,11,4,12,false),
				//TurretEncoder = new Encoder(4, 13, 4, 14, false);		// TurretEncoder(4, 13, 4, 14, false),
		dprintf(LOG_INFO,"RedAlert: Drive Encoders Initialized.");
				counter = 0;
				//		counter(0),
		dprintf(LOG_INFO,"RedAlert: Initializing Gyroscope");		
				testGyro = NULL;
						//testGyro(NULL),
		
		dprintf(LOG_INFO,"RedAlert: Gyroscope Initialized.");
		dprintf(LOG_INFO,"RedAlert: Initializing Gyroscope Temperature Sensor");		
				testTemp = NULL;
						//testTemp(NULL),
		dprintf(LOG_INFO,"RedAlert: Gyroscope Temperature Sensor Initialized.");
		dprintf(LOG_INFO,"RedAlert: Initializing Personality");
				Squeeky = NULL;
						//Squeeky(NULL),
		dprintf(LOG_INFO,"RedAlert: Personality Initialized");
		dprintf(LOG_INFO,"RedAlert: Initializing Turret");
				TheTurret = new Turret();
						//TheTurret(), 
				dprintf(LOG_INFO,"RedAlert: Turret Initialized");		
				// TODO SHould these be CurrentSensors?
				dprintf(LOG_INFO,"RedAlert: Initializing Current Sensors");
				//leftCurrent = new AnalogChannel(2,4);
						//leftCurrent(2, 4), 
				leftCurrent = new CurrentSensor();
				leftCurrent->Init(2,4,2.5,CurrentSensor::m_30Amp);
				rightCurrent = new CurrentSensor();
				rightCurrent->Init(2,5,2.5,CurrentSensor::m_30Amp);
				//rightCurrent = new AnalogChannel(2,5);
						//rightCurrent(2, 5),
				
				turretPositionCurrent = new AnalogChannel(2,3);
				
						//turretPositionCurrent(2, 3),
				dprintf(LOG_INFO,"RedAlert: Current Sensors Initialized");
				dprintf(LOG_INFO,"RedAlert: Initializing Accelerometers");
				testAccel_X = new Accelerometer(1,3);
				
				//		testAccel_X(1, 3), 
				testAccel_Y = new Accelerometer(1,4);	//		testAccel_Y(1, 4),
				testAccel_Z = new Accelerometer(1,5);	//		testAccel_Z(1, 5),
				
				testAccel_Y->SetZero(1.5);
				testAccel_X->SetZero(1.5);
				testAccel_Z->SetZero(1.5);
				
				testAccel_X->SetSensitivity(0.3);
				testAccel_Y->SetSensitivity(0.3);
				testAccel_Z->SetSensitivity(0.3);
						//				Gate(8,1),
				dprintf(LOG_INFO,"RedAlert: Accelerometers Initialized");
				dprintf(LOG_INFO,"RedAlert: Initializing Ultrasonic Sensor");
				ultrasonic = new AnalogChannel(1,6);
				dprintf(LOG_INFO,"RedAlert: Ultrasonic Sensor Initialized ... [PONG]");
				dprintf(LOG_INFO,"RedAlert: Initializing Autonomous Program");
				AutoProgram = new Autonomous();
				dprintf(LOG_INFO,"RedAlert: Autonomous Program Initialized");
				dprintf(LOG_INFO,"RedAlert: Initializing Harvester");
				Harvester = new RobotHarvester(4,6,2,6); //Harvester(4, 6, 2, 6),
				dprintf(LOG_INFO,"RedAlert: Harvester Initialized");
				dprintf(LOG_INFO,"RedAlert: Initializing Elevator");
				Elevator = new RobotElevator();			//Elevator(),
				dprintf(LOG_INFO,"RedAlert: Elevator Initialized");
				//Collector = NULL;		// Collector(8,1),
				dprintf(LOG_INFO,"RedAlert: Initializing Gate Controller");
				TheGate = new GateController();	//		TheGate(),
				dprintf(LOG_INFO,"RedAlert: Gate Controller Initialized");
				dprintf(LOG_INFO,"RedAlert: Initializing Cell Catcher");
				ECellCatcher = new CellCatcher; //		ECellCatcher(),
				dprintf(LOG_INFO,"RedAlert: Cell Catcher Initialized");
				dprintf(LOG_INFO,"RedAlert: Initializing Toggles");
				Trigger = new Toggle(leftStick, 2);	//		Trigger(&leftStick, 2), 
				GateToggle = new Toggle(turretStick, 2); //		GateToggle(&turretStick, 2), 
				CellCatcherToggle = new Toggle(rightStick, 1); //		CellCatcherToggle(&rightStick, 1),
				RunStopToggle = new Toggle(turretStick, 11); //		RunStopToggle(&turretStick, 11),
				dprintf(LOG_INFO,"RedAlert: Toggles Initialized");
				dprintf(LOG_INFO,"RedAlert: Initializing Launcher");
				launcher = new Launcher();	//		launcher(),
						// No furthur control is really necessary.
						// Pressure switch, slot 6 channel 11
						// Spike relay, slot 4 channel 1
				dprintf(LOG_INFO,"RedAlert: Launcher Initialized");
				TheCompressor = new Compressor(6, 11, 4, 1);		//TheCompressor(6, 11, 4, 1),
				dprintf(LOG_INFO,"RedAlert: Compressor Initialized");
				//RFollowWheelEncoder = new Encoder(4, 7, 4, 8,false);	//		RFollowWheelEncoder(4, 7, 4, 8,false),
				//LFollowWheelEncoder = new Encoder(4, 5, 4, 6, false); 	//		LFollowWheelEncoder(4, 5, 4, 6, false)
				dprintf(LOG_INFO,"RedAlert: FollowWheelEncoders Initialized");
				
				r_state = new RobotState();
				dprintf(LOG_INFO,"RedAlert: Robot Logging State Packer Initialized.");
				
		dprintf(LOG_INFO, "RedAlert: ////// Initializing Initializing //////");
		///////////////////////////////////////////////////////////
		// ROBOT "GOOD STATE" INITIALIZATION
		MasterControlMode = MODE_MANUAL;
		MasterProgramNumber = 0;
		dprintf(LOG_INFO, "RedAlert: Mode set to Manual");
		GetWatchdog().SetExpiration(100);
		dprintf(LOG_INFO, "RedAlert: Watchdog started, 100 mseconds. Grrr!");
		dprintf(LOG_INFO, "RedAlert: Starting launcher module...");
		launcher->Init(4, 2);
		launcher->SetRun(true);
		launcher->SetJoyStick(turretStick);

		dprintf(LOG_INFO, "RedAlert: Launcher module started.");
		AutoModeRunStop = false;
		dprintf(LOG_INFO, "RedAlert: Starting Gyros... DO NOT MOVE THE ROBOT");
		if (testGyro==NULL){
			testGyro = new Gyro(1,1);
			testGyro->SetSensitivity(.007);
		}
		if (testTemp==NULL)
			testTemp = new AnalogChannel(1,2);
		dprintf(LOG_INFO, "RedAlert: Gyros started.");
		if (StartCameraTask(13, 0, k320x240, ROT_0)==-1) {
			dprintf(LOG_INFO,"Things screwed up with camera.\n");
		}

		if (theCamera->StatusIsFatal()) {
			dprintf(LOG_INFO,"PCVideoServer is mortally wounded.\n");
		}
		
		dprintf(LOG_INFO,"RedAlert: Initializing IFF module...");
		IdentFriendFoe = new IFF();
		dprintf(LOG_INFO,"RedAlert: IFF complete.");
		// This should probably be in the Elevator code

		//if(!Gate.Get()) // Is the gate open?
		//	Gate.Set(false); // Yes? Then close it.

		dprintf(LOG_INFO,"RedAlert: Starting Harvester module...");
		Harvester->Init();
		Harvester->SetCollectEjectControls(rightStick, 4);
		Harvester->SetRunStopControls(rightStick, 3);
		Harvester->SetGateControls(TheGate);
		Harvester->SetAutoMode(MasterControlMode);
		dprintf(LOG_INFO,"RedAlert: Harvester module started.");
		//Harvester.SetAutoMode(MODE_AUTO);

		// Initiallize the Elevator class
		dprintf(LOG_INFO,"RedAlert: Starting Elevator module...");
		Elevator->Init(4, 1, 2, 1, Harvester, TheGate);
		//JDM: Set the joystick and button to use to test the elevator
		Elevator->SetElevatorControls(turretStick, 1);
		Elevator->SetAutoMode(MasterControlMode);
		dprintf(LOG_INFO,"RedAlert: Elevator module started.");
		dprintf(LOG_INFO,"RedAlert: Turret module starting.");
		TheTurret->InitTurret(4,3,	// Turret jaguar
							6,9,	// CW Limit Switch
							6,10,	// CCW Limit Switch
							1,7,	// Potentiometer Encoder
							IdentFriendFoe,	// IFF
							8);		// DS IO pin for Goggle
		dprintf(LOG_INFO,"RedAlert: Turret module started. I got you suckas'!");
		dprintf(LOG_INFO,"RedAlert: ECellCatcher module starting.");
		ECellCatcher->Init(8, 3, 8, 4);
		dprintf(LOG_INFO,"RedAlert: ECellCatcher module started.");
		
		dprintf(LOG_INFO,"RedAlert: Squeeky initializing...");
		Squeeky = new Personality();
		dprintf(LOG_INFO,"RedAlert: Squeeky is alive. And kicking.");
		dprintf(LOG_INFO,"RedAlert: Starting Compressor task...");
		TheCompressor->Start();
		dprintf(LOG_INFO,"RedAlert: Starting all them encoders...");
		// LaunchEncoder.Start();
//		TurretEncoder->Start();
		RMQuadEncoder->Start();
		LMQuadEncoder->Start();
		RMWheelEncoder->Start();
		LMWheelEncoder->Start();
		//RFollowWheelEncoder->Start();
		//LFollowWheelEncoder->Start();

		dprintf(LOG_INFO,"RedAlert: ////// DONE //////");
		dprintf(LOG_INFO,"Robot initialized.");
		MasterOperatingMode = 0;
	}

	// Disabled state methods
	void DisabledInit(void) {
		MasterOperatingMode = 1;
		MasterControlMode = MODE_MANUAL;
		MasterProgramNumber = 0;
		dsLCD = DriverStationLCD::GetInstance();
//		Squeeky->Open();
//		Squeeky->SqueekySayHello();
		if (testGyro==NULL)
			testGyro = new Gyro(1,1);

		UpdateDashboard();

	}

	void DisabledPeriodic(void) {
		MasterOperatingMode = 2;
		CheckMode();
		CheckProgram();
		//Squeeky->RPTCommandProccessor();
		UpdateDashboard();
	}

	void DisabledContinuous(void) {
	}

	// Autonomous state methods
	void AutonomousInit(void) {
		MasterOperatingMode = 3;
		Squeeky->Close();
		LMQuadEncoder->Stop();
		GetWatchdog().SetEnabled(false);
		// logger.OpenFile("log.log");
		// logger.Debug("Entering teleop mode...");
		// logger.CloseFile();

//		light->Set(0);
		//CheckProgram();
		AutoProgram->Init();
		AutoProgram->SetControls(myRobot);
		AutoProgram->SetProgramNumber(MasterProgramNumber);
		dsLCD->Printf(DriverStationLCD::kMain_Line6, 15, "Pgm: %1u*", MasterProgramNumber);
		dsLCD->UpdateLCD();
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
		MasterOperatingMode = 4;
		AutoProgram->Periodic();
		TheTurret->TurretControl(); // This updates the Target in Sight member variable;

		if (TheTurret->TargetInSight()) {
			//GetTheDashboard().Printf("TARGET IN SIGHT!");
			FetchBoy(TheTurret->GetTarget_X());
		} else {
			//GetTheDashboard().Printf("WHERE?!");
			myRobot->Drive(0.0, 0.0);
		}
		UpdateDashboard();
	}

	void AutonomousContinuous(void) {
		// UpdateDashboard();
		AutoProgram->Continuous();
	}

	// Teleop state methods
	void TeleopInit(void) {
		MasterOperatingMode = 5;
		//		testEncoder.SetDistancePerTick(300.0);
		//		LMQuadEncoder.SetDistancePerPulse(300.0);
		//LMQuadEncoder->Start();
		if (testGyro==NULL)
			testGyro = new Gyro(1,1);
	//	light->Set(1);
		// myRobot.Drive(0.0, 0.0); // stop robot
		// myRobot.Drive(0.5, 0.0); // Go Straight Forward!

		Harvester->SetAutoMode(MasterControlMode);
		Elevator->SetAutoMode(MasterControlMode);
		TheTurret->SetMode(MasterControlMode);
//		TheTurretSetMode(MODE_MANUAL);
//		//TheTurret.RegisterMasterControl(&MasterControlMode);
		Squeeky->Close();
		GetWatchdog().SetEnabled(true);
//#warning "The Watchdog NEEDS to be fed."
		
		//GetWatchdog().SetEnabled(false);
		// Launch_Wheels_Motor.Set(0.0);
	}

	bool CheckMode(void)
	{
		int DSMode = 0;
		bool ModeChanged;
		//DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();

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
			MasterControlMode = MODE_SEMI_AUTO;
		}

		switch (DSMode) {
		case MODE_MANUAL:
			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Manual");
			break;
		case MODE_SEMI_AUTO:
			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Semi-A");
			break;
		case MODE_AUTO:
			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Full-A");
			break;
		default:
			dsLCD->Printf(DriverStationLCD::kMain_Line6, 1, "Mode: Error ");
		}
		dsLCD->UpdateLCD();
		return ModeChanged;
	}
	
	bool CheckProgram(void)
	{
		UINT32 ProgramNumber = 0;
		bool ProgramChanged;
		//DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();

		ProgramNumber = (UINT32)(m_ds->GetDigitalIn(AUTONOMOUS_PROGRAM_SWITCH_1) * 2 + m_ds->GetDigitalIn(AUTONOMOUS_PROGRAM_SWITCH_2));
		ProgramNumber--; // Decrement program number to cooperate with arrays in autonomous.
		ProgramChanged = (ProgramNumber == MasterProgramNumber);
		
		switch (ProgramNumber) {
		case 0:
			MasterProgramNumber = ProgramNumber;
			break;
		case 1:
			MasterProgramNumber = ProgramNumber;
			break;
		case 2:
			MasterProgramNumber = ProgramNumber;
			break;
		default:
			MasterProgramNumber = 0;
			dsLCD->Printf(DriverStationLCD::kMain_Line6, 15, "Pgm:(0)");
		}
		dsLCD->Printf(DriverStationLCD::kMain_Line6, 15, "Pgm:%1u %1u", MasterProgramNumber, ProgramNumber);
		dsLCD->UpdateLCD();
		return ProgramChanged;
	}

	void TeleopPeriodic(void) {

		GetWatchdog().Feed();
		MasterOperatingMode = 6;
		bool shouldLimit = rightStick->GetRawButton(2);
		
		float left_y =  - leftStick->GetY();
		float right_y = - rightStick->GetY();
		
		if (shouldLimit && fabs(right_y) >= .3) {
			//leftstick_limit->StartNewIteration(left_y);
			//rightstick_limit->StartNewIteration(right_y);
			
		} else {
			leftstick_limit->NotifyWhileIgnored(left_y);
			rightstick_limit->NotifyWhileIgnored(right_y);
		}
		leftstick_limit->StartNewIteration(left_y);
		rightstick_limit->StartNewIteration(right_y);
		
		float left_y_correct = leftstick_limit->GetCorrectedInput();
		float right_y_correct = rightstick_limit->GetCorrectedInput();
		
		//myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)

		// testMotor.SetRaw((UINT8)floor(test.GetRawAxis( Joystick::kDefaultYAxis )));
		//testMotor.Set(test.GetY());
		// determine if tank or arcade mode; default with no jumper is for tank drive
		if (m_ds->GetDigitalIn(ARCADE_MODE) == 1) {
			// myRobot.TankDrive(leftStick, rightStick);	 // drive with tank style

			// This is a quick temporary for the inverted Y axis Mr. Meyer,
			// but I think the issue with the inverted joysticks is with the motors...
			// they may need to have the wires on their polarity switched.
			//myRobot->TankDrive(-leftStick->GetY(), -rightStick->GetY());
			myRobot->TankDrive(left_y_correct, right_y_correct);
		} else {
			myRobot->ArcadeDrive(right_y_correct, -rightStick->GetX()); // drive with arcade style (use right stick)
		}

		// Just for testing, comment if you don't want it.
		//testMotor.Set(testMotorStick.GetY()); // Set Test Motor based on Y Axis


		RunStopToggle->UpdateState();
		if(RunStopToggle->GetOutput())
			AutoModeRunStop = true;
		else
			AutoModeRunStop = false;
		
//		if (testMotorStick->GetButton(testMotorStick->kTopButton)) {
//			LMQuadEncoder->Reset();
//			delete testGyro;
//			testGyro = new Gyro(1,1);
//		}

//		// Check to see if the Master Control Mode has changed
//		// If so, update the modes for the Harvester, Elevator, and Turret
//		if (CheckMode())
//		{
//			Harvester.SetAutoMode(MasterControlMode);
//			Elevator.SetAutoMode(MasterControlMode);
//			TheTurret.SetMode(MasterControlMode);
//		}

		//TheTurret.SetMode(MODE_)
		Elevator->Process(launcher->GetStatus(), AutoModeRunStop);
		// JDM: Use joystick to test, needs to use Elevator Load flag
		
		Harvester->Process(Elevator->GetHarvesterLoad(), AutoModeRunStop);
		launcher->Update();

		if(MasterControlMode == MODE_MANUAL || MasterControlMode == MODE_SEMI_AUTO && !Elevator->IsCycling())
			TheGate->Set(GateToggle->GetOutput());

		DriverStationLCD * dlcd = DriverStationLCD::GetInstance();
		dlcd->Printf(DriverStationLCD::kUser_Line4, 5, "%1d",
				GateToggle->GetOutput());

		ECellCatcher->Set(CellCatcherToggle->GetOutput());
		GateToggle->UpdateState();
		CellCatcherToggle->UpdateState();

		//#warning "THIS IS TEST CODE. DON'T SEND THE ROBOT INTO COMPETITION"

		// Map X-Axis of joystick to turret position
		//Turret_Pos_Motor.Set(turretStick.GetX());
		TheTurret->TurretControl(turretStick);

		// Moved to elevator code.
		// Launch_Wheels_Motor.Set(-((-turretStick.GetZ()+1.0)/2.0));


		// horizontalServo.Set((turretStick.GetX()+ 1.0) / 2.0);
		// verticalServo.Set((turretStick.GetY()+ 1.0) / 2.0);

		AirCompressorControl(); // This doesn't really do anything
		UpdateDashboard();
	}

	void TeleopContinuous(void) {
		GetWatchdog().Feed();
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

		
		////UPDATE ROBOT STATE////
		
		TheTurret->ReportState(r_state);
		Elevator->ReportState(r_state);
		launcher->ReportState(r_state);
		Harvester->ReportState(r_state);
		
		
		
		if (!TheTurret->TargetInSight()) {
			dashboardDataFormat->m_TopBoundRect = 1;
			dashboardDataFormat->m_BottomBoundRect = 2;
			dashboardDataFormat->m_LeftBoundRect = 1;
			dashboardDataFormat->m_RightBoundRect = 2;
		} else {
			dashboardDataFormat->m_TopBoundRect = TheTurret->TargetData().boundingRect.top;
			dashboardDataFormat->m_LeftBoundRect = TheTurret->TargetData().boundingRect.left;
			dashboardDataFormat->m_BottomBoundRect = TheTurret->TargetData().boundingRect.top + TheTurret->TargetData().boundingRect.height;
			dashboardDataFormat->m_RightBoundRect = TheTurret->TargetData().boundingRect.left + TheTurret->TargetData().boundingRect.width;
		}
		//dashboardDataFormat.m_AnalogChannels[0][4] = testMotor.Get();
		dashboardDataFormat->m_DIOChannels[0]++;
		dashboardDataFormat->m_DIOChannelsOutputEnable[0]--;
		dashboardDataFormat->m_RM_QuadEncoder = RMQuadEncoder->Get();
		dashboardDataFormat->m_LM_QuadEncoder = LMQuadEncoder->Get();
		
		r_state->SetQuadEncoder(RobotState::LeftMotor, LMQuadEncoder->Get(), 0);
		r_state->SetQuadEncoder(RobotState::RightMotor, RMQuadEncoder->Get(), 0);
		r_state->SetQuadEncoder(RobotState::LeftFollow, LMWheelEncoder->Get(), 0);
		r_state->SetQuadEncoder(RobotState::RightFollow, RMWheelEncoder->Get(), 0);
		
		r_state->SetCurrent(RobotState::LeftSideDriverCurrent, leftCurrent->GetCurrent());
		r_state->SetCurrent(RobotState::RightSideDriverCurrent, rightCurrent->GetCurrent());

		r_state->SetUltrasonicVoltage(ultrasonic->GetVoltage());
		
		//dashboardDataFormat->m_TurretEncoder = TurretEncoder->Get();

		r_state->SetAccelerometerAxis(RobotState::XAxis, testAccel_X->GetAcceleration());
		r_state->SetAccelerometerAxis(RobotState::YAxis, testAccel_Y->GetAcceleration());
		r_state->SetAccelerometerAxis(RobotState::ZAxis, testAccel_Z->GetAcceleration());
		
		/*
		dashboardDataFormat->m_accelX = testAccel_X->GetAcceleration();
		dashboardDataFormat->m_accelY = testAccel_Y->GetAcceleration();
		dashboardDataFormat->m_accelZ = testAccel_Z->GetAcceleration();
		*/
		//dashboardDataFormat->m_gyroTemp = testTemp->GetAngle();
		
		r_state->SetGyroData(0.0, testTemp->GetVoltage() , testGyro->GetAngle());
		
		r_state->ReportJoystick(RobotState::Left, leftStick);
		r_state->ReportJoystick(RobotState::Right, rightStick);
		r_state->ReportJoystick(RobotState::Turret, turretStick);
//		if (testGyro == NULL) {
//			dashboardDataFormat->m_gyroAngle = -42.0001;
//		} else {
//			dashboardDataFormat->m_gyroAngle = testGyro->GetAngle();
//			//dashboardDataFormat.m_gyroAngle = 589.7;
//		}
//		//  dashboardDataFormat.m_accelX = 84.0;

		r_state->ReportOperatingMode(MasterOperatingMode);
		r_state->ReportServiceMode((UINT32)(MasterControlMode));
		r_state->ReportAutonomousProgram(MasterProgramNumber);
		dashboardDataFormat->UltraSonic01RawData = (INT32)ultrasonic->GetValue();
		
		dashboardDataFormat->m_TurretState = 0;
		dashboardDataFormat->m_LeftState = 0;
		dashboardDataFormat->m_RightState = 0;

		dashboardDataFormat->m_TurretPotentEncoderVoltage
				= TheTurret->EncoderVoltage();

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
		
		
		r_state->PackData(dashboardDataFormat);
		//dashboardDataFormat->m_left_drive_amps = 3.14159;
		dashboardDataFormat->PackAndSend();
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

