#ifndef TURRET_H__
#define TURRET_H__

#include "WPILib.h"
#include "Target.h"
#include "LimitSwitch.h"


#define ENABLE_TURRET 1

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
	
	float CurrentPosition(void);
	float EncoderVoltage(void);
	
	// Am I a bad person? >:)
	// Yes. --PF
	
	
	void SetTurretPosition(float position);
	float TurretPosition(void);
	
	
	void SetMode(int m)
	{
		this->mode = mode;
	}
	
	ParticleAnalysisReport TargetData() const { return par; }

	
private:
	//PurpleMonkeys *theRobot;
	
	// Code that has been refactored to deal with targeting
	void Target();
	
	void Manual(Joystick *turretStick);
	void ManualPositionMode(Joystick *turretStick);
	void Auto(void); // Autonomous does not need joystick, neither should auto
	
	void Track(void);
	
	void UpdateState(void);
	
	void InitServoish(void);
	
	void EndServoish(void);
	
	float ServoToEncoderUnits(float servo);
	float EncoderUnitsToServo(float volts);
	
	void AdjustServoPositions(float dH, float dV);
	
	////////////////////////////////////////////////////////
	// HARDWARE
	
	Jaguar *Turret_Pos_Motor;
	LimitSwitch *Clockwise_Limit, *CounterClockwise_Limit;
	AnalogChannel *Position_Encoder;
	
	static const float kCCWVoltage = 1.7;
	static const float kCWVoltage = 3.6;
	static const float kEncoderRange = 1.9;
	////////////////////////////////////////////////////////
	// STATE
	
	float max_encoder_voltage;
	float min_encoder_voltage;
	bool tracking;
	
	int mode;
	TrackingThreshold td1, td2;		// color thresholds	
	TrackingThresholdRGB custom1, custom2; // Custom color thresholds
	
	ParticleAnalysisReport par;		// particle analysis report
	
	PIDController *pid;
	PIDSource *pid_src;
	
	int *masterControl;
	
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
