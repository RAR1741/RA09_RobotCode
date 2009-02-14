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
#include "Toggle.h"
#include "Harvester.h"
#include "Elevator.h"
#include "Launcher.h"

#define USR_RQ_EJECT 0
#define USR_RQ_IDLE 1

#define USR_RQ_RUN true
#define USR_RQ_STOP false

BOOL ToggleSwitch (BOOL Input, unsigned int *StateVariable);  // Proto for toggle function testing HAM 2/11/2009
/*
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
	// Encoder testEncoder;
	Encoder LMQuadEncoder;
	Encoder RMQuadEncoder;
	Encoder LMWheelEncoder;
	Encoder RMWheelEncoder;
	Encoder LaunchEncoder;
	Encoder ElevatorEncoder;
	Encoder TurretEncoder;
	
	//Victor testMotor;
	//Servo horizontalServo;
	//Servo verticalServo;
	int counter;
	Gyro * testGyro;
	Gyro * testTemp;
	// bool printed;
	// Autonomous Auto;
	Personality * Squeeky;
	Turret TheTurret;
	AnalogChannel leftCurrent;
	AnalogChannel rightCurrent;
	//AnalogChannel elevatorCurrent;
	AnalogChannel launchWheelsCurrent;
	AnalogChannel turretPositionCurrent;
	//AnalogChannel harvesterCurrent;
	
	Accelerometer testAccel_X,testAccel_Y,testAccel_Z;
	//Jaguar Harvester_Motor;
	// Jaguar Launch_Wheels_Motor;
	// Jaguar Turret_Pos_Motor; // Outsourced to Turret.h/cpp by PJF at 20:28 02/09/2009
	//Jaguar Elevator_Motor;
	Solenoid Gate;
	//Toggle HarvesterToggle;
	//Toggle EjecterToggle;
	RobotHarvester Harvester;
	// RobotElevator Elevator;
	Solenoid Collector;
	Toggle Trigger;
	Launcher launcher;
	// State Variables for toggle code.
	
	//// Harvester State Vars
	//unsigned int UserRequestHarvesterEjectIdle;
	//unsigned int UserRequestHarvesterRunStop;
	// Elevator state vars
	unsigned int UserRequestElevatorArmDisarm;
	unsigned int UserRequestElevatorRunStop;
	unsigned int UserRequestElevatorFire;
	enum							// Driver Station jumpers to control program operation
	{ ARCADE_MODE = 1,				// Tank/Arcade jumper is on DS Input 1 (Jumper present is arcade)
	  ENABLE_AUTONOMOUS = 2,		// Autonomous/Teleop jumper is on DS Input 2 (Jumper present is autonomous)
	} jumpers;	 
	
public:
	PurpleMonkeys(void) :
		myRobot(4, 5), // these must be initialized in the same order
				//stick(1), // as they are declared above.
				//test(2),
				leftStick(1),
				rightStick(2),
				turretStick(3),
				testMotorStick(4),
				
				light(4, 14),
				// logger(),
		  		theCamera(),
				dashboardDataFormat(),
				LMQuadEncoder(4,1,4,2,false),
				RMQuadEncoder(4,3,4,4,false),
				LMWheelEncoder(4,5,4,6,false),
				RMWheelEncoder(4,7,4,8,false),
				LaunchEncoder(4,9,4,10,false),
				ElevatorEncoder(4,11,4,12,false),
				TurretEncoder(4,13,4,14,false),
				
				// testMotor(4,3),
				//horizontalServo(4,4),
				//verticalServo(4,5),
				
				counter(0),
				testGyro(NULL),
				testTemp (NULL),
				//printed(false)
				
				//Auto()
				Squeeky(NULL),
				TheTurret(),
				leftCurrent(2,4),
				rightCurrent(2,5),
				//elevatorCurrent(2,1),
				launchWheelsCurrent(2,2),
				turretPositionCurrent(2,3),
				//harvesterCurrent(2,6),
				testAccel_X(1,3),
				testAccel_Y(1,4),
				testAccel_Z(1,5),
				//Harvester_Motor(4, 6),
				// Launch_Wheels_Motor(4,2),
//				Turret_Pos_Motor(4,3),
				//Elevator_Motor(4,1),
				Gate(8,1),
				//HarvesterToggle(&rightStick, 2),
				//EjecterToggle(&leftStick, 2),
				//RobotHarvester(UINT32 MotorSlot, UINT32 MotorChannel, UINT32 CurrentSlot, UINT32 CurrentChannel);
				Harvester(4, 6, 2, 6),
				// Elevator(4, 1, 2, 1),
				Collector(8,1),
				Trigger(&leftStick, 2),
				launcher(4,2)
				{
		// GetTrackingData(YELLOW, PASSIVE_LIGHT);
		
	}

	// StartCompetition loop
	//void StartCompetition(void)
	//{
	//}

//	void InitHarvester()
//	{
//		// HarvesterState = HARV_STATE_NOT_FULL;
//		// HarvesterAutoMode = HARV_AUTO_MODE_COLLECT;
//		
//		if(!Gate.Get()) // Is the gate open?
//			Gate.Set(false); // Yes? Then close it.
//		
//		// ElevatorState = ELEVATOR_STATE_FULL;
//		// EdgeTrigger;
//		Harvester_Motor.Set(0.0);
//		
//		//// Zero all state vars (prevents garbage bit's from getting in the way.)
//		//UserRequestHarvesterEjectIdle =
//		//UserRequestHarvesterRunStop = 0;
//		//Harvester_Motor
//	}
	
//	int ProcessHarvester(bool LoadElevator)
//	{
//		/*
//		if(LoadElevator)
//			HarvesterAutoMode = HARV_AUTO_MODE_LOAD;
//		if(EjectToggle)
//			UserRequestEjectIdle = !UserRequestEjectIdle;
//		*/
//		/*
//		if (true) { // manual mode
//			if (RunStopToggle && !LastButtonValue) {
//				UserRequestRunStop = !UserRequestRunStop;
//			} else {
//				// Maintain state
//			}
//			if (EjectToggle && !LastEjectValue) {
//				UserRequestEjectIdle = !UserRequestEjectIdle;
//			}
//
//			int direction = (UserRequestEjectIdle ? 1 : -1);
//			if (UserRequestRunStop) {
//				Harvester_Motor.Set(.5 * direction);
//			} else {
//				Harvester_Motor.Set(0.0);
//			}
//		}
//		
//		LastButtonValue = RunStopToggle;
//		LastEjectValue = EjectToggle;
//		
//		*/
//		/*
//		if (RunStopToggle) {
//			Harvester_Motor.Set(.5);
//		} else if (EjectToggle) {
//			Harvester_Motor.Set(-.5);
//		}
//		*/
//		
//		HarvesterToggle.UpdateState();
//		EjecterToggle.UpdateState();
//
//		if(1){// Put Manual/Auto if condition here. 
//			// Manual Mode
//			if (HarvesterToggle.GetOutput()){
//				Harvester_Motor.Set(.5);
//				EjecterToggle.Reset();
//			}
//			else{
//				if(EjecterToggle.GetOutput()){
//					Harvester_Motor.Set(-.5);
//				}
//				else{
//					Harvester_Motor.Set(0.0);
//				}
//			}
//		}
//		else{
//			// Auto mode code should be here.
//		}
//		
//		return 0;
//	}
	
//	int ProcessHarvester(BOOL RunStopToggle, BOOL EjectToggle, bool LoadElevator)
//	{
//		/*
//		if(LoadElevator)
//			HarvesterAutoMode = HARV_AUTO_MODE_LOAD;
//		if(EjectToggle)
//			UserRequestEjectIdle = !UserRequestEjectIdle;
//		*/
//		/*
//		if (true) { // manual mode
//			if (RunStopToggle && !LastButtonValue) {
//				UserRequestRunStop = !UserRequestRunStop;
//			} else {
//				// Maintain state
//			}
//			if (EjectToggle && !LastEjectValue) {
//				UserRequestEjectIdle = !UserRequestEjectIdle;
//			}
//
//			int direction = (UserRequestEjectIdle ? 1 : -1);
//			if (UserRequestRunStop) {
//				Harvester_Motor.Set(.5 * direction);
//			} else {
//				Harvester_Motor.Set(0.0);
//			}
//		}
//		
//		LastButtonValue = RunStopToggle;
//		LastEjectValue = EjectToggle;
//		
//		*/
//		/*
//		if (RunStopToggle) {
//			Harvester_Motor.Set(.5);
//		} else if (EjectToggle) {
//			Harvester_Motor.Set(-.5);
//		}
//		*/
//		
//		BOOL RSToggle = ToggleSwitch(RunStopToggle, &UserRequestHarvesterRunStop);
//		BOOL EJToggle;
//		
//		// Only toggle eject if harvester is off.
//		if(RSToggle==false)
//			EJToggle = ToggleSwitch(EjectToggle, &UserRequestHarvesterEjectIdle);
//		if(1){// Put Manual/Auto if condition here. 
//			// Manual Mode
//			if(RSToggle){
//				Harvester_Motor.Set(.5);
//			}
//			else{
//				if(EJToggle){
//					Harvester_Motor.Set(-.5);
//				}
//				else{
//					Harvester_Motor.Set(0.0);
//				}
//			}
//		}
//		else{
//			// Auto mode code should be here.
//		}
//		
//		// LastButtonValue = RunStopToggle;
//
//		return 0;
//	}
	
//	void InitElevator()
//	{
//		
//	}

	// One time initialization of the robot
	void RobotInit(void) {
		GetWatchdog().SetExpiration(100);
		launcher.SetJoyStick(&turretStick);
		// Elevator.Init(&launcher);
		if(testGyro==NULL)
			testGyro = new Gyro(1,1);
		if(testTemp==NULL)
			testTemp = new Gyro(1,2);
		 if(StartCameraTask(13,0,k320x240,ROT_0)==-1)
		 {
			 dprintf("Things screwed up with camera.\n");
		 }
		 
		 if(theCamera.StatusIsFatal()){
			 dprintf("PCVideoServer is mortally wounded.\n");
		 }
		   dprintf("Robot initialized.");
		   
		  //InitHarvester();
			
		// This should probably be in the Elevator code
		if(!Gate.Get()) // Is the gate open?
			Gate.Set(false); // Yes? Then close it.
		
		Harvester.Init();
		Harvester.SetHarvesterControls(&rightStick, 2);
		Harvester.SetEjecterControls(&leftStick, 2);
		
		// Elevator.Init(&launcher);
		//JDM: Set the joystick and button to use to test the elevator
		// Elevator.SetElevatorControls(&turretStick, 2);

		Squeeky = new Personality();
	}

	// Disabled state methods
	void DisabledInit(void) {
		Squeeky->Open();
		Squeeky->SqueekySayHello();
		if(testGyro==NULL)
			testGyro = new Gyro(1,1);
		
		UpdateDashboard();
		
	}

	void DisabledPeriodic(void) {
		//Squeeky->RPTCommandProccessor();
	}

	void DisabledContinuous(void) {
	}

	// Autonomous state methods
	void AutonomousInit(void) {
		Squeeky->Close();
		LMQuadEncoder.Stop();
		//testMotor.Set(0.0);
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
	void FetchBoy(double x_val)
	{
		// Decide How to turn
	}

	void AutonomousPeriodic(void) {
		
		// static int counter = 0;
		// counter++;
		// if (counter>=400)
		//	myRobot.Drive(0.0, 0.0);
		// Auto.Periodic();
		TheTurret.TurretControl(); // This updates the Target in Sight member variable;
		
		if(TheTurret.TargetInSight())
		{
			GetTheDashboard().Printf("TARGET IN SIGHT!");
			FetchBoy(TheTurret.GetTarget_X());
		}
		else
		{
			GetTheDashboard().Printf("WHERE?!");
			myRobot.Drive(0.0,0.0);
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
		if(testGyro==NULL)
			testGyro = new Gyro(1,1);
		light.Set(1);
		// myRobot.Drive(0.0, 0.0); // stop robot
		// myRobot.Drive(0.5, 0.0); // Go Straight Forward!
		GetWatchdog().SetEnabled(true);
		// Launch_Wheels_Motor.Set(0.0);
	}

	void TeleopPeriodic(void) {

		GetWatchdog().Feed();
		
		//myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
		
		// testMotor.SetRaw((UINT8)floor(test.GetRawAxis( Joystick::kDefaultYAxis )));
		//testMotor.Set(test.GetY());
		// determine if tank or arcade mode; default with no jumper is for tank drive
		if (m_ds->GetDigitalIn(ARCADE_MODE) == 0) {	
			// myRobot.TankDrive(leftStick, rightStick);	 // drive with tank style
			myRobot.TankDrive( leftStick.GetY(), rightStick.GetY());
		} else{
			myRobot.ArcadeDrive(rightStick.GetY(), - rightStick.GetX());	         // drive with arcade style (use right stick)
		}
		
		// Just for testing, comment if you don't want it.
		//testMotor.Set(testMotorStick.GetY()); // Set Test Motor based on Y Axis
		
		
		if(testMotorStick.GetButton(testMotorStick.kTopButton))
			{
			LMQuadEncoder.Reset();
				delete testGyro;
				testGyro = new Gyro(1,1);
			}
		
		/*
		bool RunStop,  EjectToggle;

		if(leftStick.GetButton(leftStick.kTopButton))
			EjectToggle = true;
		else
			EjectToggle = false;
		
		if(rightStick.GetButton(rightStick.kTopButton))
			RunStop = true;
		else
			RunStop = false;
		*/
		
		// Elevator.Process();
		Harvester.Process(false);
		launcher.Update();
		//ProcessHarvester(false);
		//ProcessHarvester(RunStop, EjectToggle, false);
		
		//
		// 
		//
		
//#warning "THIS IS TEST CODE. DON'T SEND THE ROBOT INTO COMPETITION"
		
		/*
		
		// Harvester for testing is run off of top button.
		if (turretStick.GetButton(turretStick.kTopButton)) {
			this->Harvester_Motor.Set(1.0);
		} else {
			this->Harvester_Motor.Set(0.0);
		}
		*/
		// Map X-Axis of joystick to turret position
		//Turret_Pos_Motor.Set(turretStick.GetX());
		TheTurret.TurretControl(&turretStick);
		
		// Moved to elevator code.
		// Launch_Wheels_Motor.Set(-((-turretStick.GetZ()+1.0)/2.0));
	
		
		// horizontalServo.Set((turretStick.GetX()+ 1.0) / 2.0);
		// verticalServo.Set((turretStick.GetY()+ 1.0) / 2.0);
		UpdateDashboard();

		if (leftStick.GetTrigger())
			Collector.Set(true);
		else
			Collector.Set(false);
	}

	void TeleopContinuous(void) {
	}
	
	 void UpdateDashboard(void)
	 {
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
	 
	 GetTheDashboard().Printf("Encoder Counts: %d, Distance: %f, Gyro Angle: %f, Left Motor Voltage: %d", dashboardDataFormat.m_RM_QuadEncoder, LMQuadEncoder.GetDistance(), testGyro->GetAngle(), dashboardDataFormat.m_LeftMotorVoltage);
	 num += 0.01;
	 if (num > 5.0) num = 0.0;
	 dashboardDataFormat.PackAndSend();
	 }
	 

	Dashboard& GetTheDashboard() {
		return m_ds->GetDashboardPacker();
	}
	
	void SeekTarget()
	{
		/*
		if ( FindTwoColors(td1, td2, ABOVE, &par) ){
			found = true;
		} else {
			found = false;
		}
		*/
	}
	
   

	
};

START_ROBOT_CLASS(PurpleMonkeys);
#endif


//int ToggleSwitch_LED1_State = 0;  // Example state variable for ToggleSwitch
//LED1_IO = ToggleSwitch(!BUTTON1_IO, &ToggleSwitch_LED1_State);  // Example use



//  ***** ToggleSwitch *****
/*  Use this function to convert a signal into a toggle function.
    Call this with an input and the output will toggle on each cycle
    of the leading edge of the input signal. You must maintain a state
    variable that will persist during use of this function. It requires
    and uses the low two bits. Pass this function the input signal
    variable and an address to an integer to use for the state variable.
    This function will return the state of the toggled output.
	By Hugh Meyer February 11, 2009
*/

// NOTE: IMPORTANT!!!
// I didn't see why HAM passed it an int pointer at first, but now I know
// it needs to be that way, to save the state outside the function.
//BOOL ToggleSwitch (BOOL Input, unsigned int *State)
//{
//  unsigned int s = *State;      // Dereference saved state to local variable
//  s = s << 1 | Input;  // Shift it left and add the input bit to it
//
//  switch (s) {         // Use it as a state variable
//
//    case 0:        // Idle State Off
//      *State = 0;  // Set NextState(0) - Spin here til the input goes to 1
//      return 0;    // Input = 0, Output = 0
//
//    case 1:        // Leading Edge Detected - Input transitioned from 0 to 1
//      *State = 1;  // Set NextState(3)
//      return 1;    // Input = 1, Output = 1
//
//    case 2:        // Trailing Edge Detected - Input transitioned from 1 to 0
//      *State = 3;  // Set NextState(6) - Return to Idle Stte On
//      return 1;    // Input = 0, Output = 1
//
//    case 3:        // Waiting for release
//      *State = 1;  // Set NextState(3) - Spin here until input goes to 0
//      return 1;    // Input = 1, Output = 1
//
//    case 4:        // Trailing Edge Detected - Input transitioned from 1 to 0
//      *State = 0;  // Set NextState(0) - Return to Idle State Off
//      return 0;    // Input = 0, Output = 0
//
//    case 5:        // Waiting for release
//      *State = 2;  // Set NextState(4) - Spin here until input goes to 0
//      return 0;    // Input = 1, Output = 0
//
//    case 6:        // Idle State On
//      *State = 3;  // Set NextState(6) - Spin here til the input goes to 1
//      return 1;    // Input = 1, Output = 1
//
//    case 7:        // Leading Edge Detected - Input transitioned from 0 to 1
//      *State = 2;  // Set NextState(5)
//      return 0;    // Input = 1, Output = 0
//
//    default:
//      return 0;
//  }    
//
//}	
//  ***** End of ToggleSwitch *****




