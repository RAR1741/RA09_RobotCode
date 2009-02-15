#ifndef TURRET_H__
#define TURRET_H__

#include "WPILib.h"
#include "Target.h"
#include "LimitSwitch.h"



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
	
	
	ParticleAnalysisReport TargetData() const { return par; }

	
private:
	//PurpleMonkeys *theRobot;
	
	// Code that has been refactored to deal with targeting
	void Target();
	
	void Manual(Joystick *turretStick);
	void ManualPositionMode(Joystick *turretStick);
	void Auto(void); // Autonomous does not need joystick, neither should auto

	void UpdateState(void);
	
	void InitServoish(void);
	
	void EndServoish(void);
	
	float ServoToEncoderUnits(float servo);
	float EncoderUnitsToServo(float volts);
	
	////////////////////////////////////////////////////////
	// HARDWARE
	
	Jaguar *Turret_Pos_Motor;
	LimitSwitch *Clockwise_Limit, *CounterClockwise_Limit;
	AnalogChannel *Position_Encoder;
	
	////////////////////////////////////////////////////////
	// STATE
	
	float max_encoder_voltage;
	float min_encoder_voltage;
	bool tracking;
	TrackingThreshold td1, td2;		// color thresholds	
	TrackingThresholdRGB custom1, custom2; // Custom color thresholds
	
	ParticleAnalysisReport par;		// particle analysis report
	
	PIDController *pid;
	PIDSource *pid_src;
	
	int *masterControl;
};

#endif
