#ifndef TURRET_H__
#define TURRET_H__

#include "WPILib.h"
#include "Target.h"
#include "LimitSwitch.h"
#include "IFF.h"

#include "PotentiometerEncoder.h"
#include "JoystickPIDSource.h"
#include "CameraPIDSource.h"
#include "DriverStation.h"
#include "CurrentSensor.h"

#include "RobotState.h"
#define ENABLE_TURRET 0

// #include "Robot.cpp"

class Turret
{
public:
	Turret();
	~Turret();
	
	void RegisterMasterControl(int *mcontrol);
	void TurretControl(void);
	void TurretControl(Joystick *turret);
	
	TrackingThreshold PrimaryThreshold() const { return td1; };
	TrackingThreshold SecondaryThreshold() const { return td2; };
	bool TargetInSight() const { return tracking; };
	double GetTarget_X(); // HaHa Patrick! Added a function to YOUR code! How does THAT make you feel?
	
	
	void InitTurret(int motorSlot, int motorChannel,
					int cLimitSlot, int cLimitChannel,
					int ccLimitSlot, int ccLimitChannel,
					int poEncoderSlot, int poEncoderChannel,
					IFF * iff_module,
					int gogglePin);
	float CurrentPosition(void);
	float EncoderVoltage(void);
	
	// Am I a bad person? >:)
	// Yes. --PF
	
	
	void SetTurretPosition(float position);
	float TurretPosition(void);
	
	
	void SetMode(int m)
	{
		this->mode = m;
	}
	
	ParticleAnalysisReport TargetData() const { return par; }
	
	// This should be implemented in other modules as well.
	void ReportState(RobotState *state);
	
private:
	//PurpleMonkeys *theRobot;
	
	// Code that has been refactored to deal with targeting
	void Target();
	
	void Manual(Joystick *turretStick);
	void ManualPositionMode(Joystick *turretStick);
	
	void ServoPositionMode(Joystick *turretStick);
	void Auto(void); // Autonomous does not need joystick, neither should auto
	
	void Track(void);
	
	// Use motor framework instead of servo
	void TrackAlt(void);
	
	void UpdateState(void);
	
	void InitServoish(void);
	
	void EndServoish(void);
	void GoToPos(float input);
	
	float ServoToEncoderUnits(float servo);
	float EncoderUnitsToServo(float volts);
	float RelativeToEncoderUnits(float rel);
	
	bool IsHoming(void) {
		return am_homing;
	}
	
	void HomeIt(void);
	
	
	
	
	// This will notify the other modules and/or drivers that
	// the turret is a happenin' place. This notably includes the
	// cool googles.
	
	void NotifySystem(void);
	void AdjustServoPositions(float dH, float dV);
	
	void Rotate(float input);
	////////////////////////////////////////////////////////
	// HARDWARE
	
	Jaguar *Turret_Pos_Motor;
	LimitSwitch *Clockwise_Limit, *CounterClockwise_Limit;
	//AnalogChannel *Position_Encoder;
	PotentiometerEncoder *Position_Encoder;
	DriverStation *ds;
	CurrentSensor *turret_current;	// 23 
#if 0
	JoystickPIDSource *stSrc;
	CameraPIDSource *caSrc;
	
	
	PIDController *joystickController;
	PIDController *cameraController;
#endif
	
//	PIDController *pid;
	
	int m_goggleLightPin;
	
	//static const float kCCWVoltage = 1.55;
	//static const float kCWVoltage = 3.35;
	//static const float kCCWVoltage = 3.35;
	//static const float kCWVoltage = 1.55;
	static const float kEncoderRange = 1.63;
	static const float kCCWVoltage = 3.33;
	static const float kCWVoltage = 1.70;
	
	static const float kCWSlowDownVoltage = 1.75;
	static const float kCCWSlowDownVoltage = 3.18;
	//static const float kSlowDownMaxOut = .5;
	
	static const float kVisibleRange = 0.5;	// Range in encoder volt units
	static const int kGoggleLightPin = 5;
	
	static const float kHomeItThreshold = 0.01;	// Has to be +- this threshold to stop.
	
	////////////////////////////////////////////////////////
	// STATE
	
	float max_encoder_voltage;
	float min_encoder_voltage;
	bool tracking;
	bool am_homing;
	int mode;
	TrackingThreshold td1, td2;		// color thresholds	
	TrackingThresholdRGB custom1, custom2; // Custom color thresholds
	
	ParticleAnalysisReport par;		// particle analysis report
	
	PIDController *pid;
	//PIDSource *pid_src;
	
	CameraPIDSource *theCam;
	PIDController *targetTrack;
	
	
	int *masterControl;
	
	IFF *m_iff_module;
	////////////////////////////////////////////////////////
	// AUTO-CONTROL State
	float incrementH, incrementV;
	// pan needs a 1-up number for each call
	int panIncrement; // = 0;							
	float sinStart;
	float horizontalDestination;	// servo destination (0.0-1.0)
	float verticalDestination;		// servo destination (0.0-1.0)
	float horizontalPosition, verticalPosition;	// current servo positions
	float servoDeadband;			// percentage servo delta to trigger move
	int framesPerSecond;			// number of camera frames to get per second
	
	double savedImageTimestamp;		// = 0.0;
	bool staleImage;				// = false; 
	
	
		
};

#endif
