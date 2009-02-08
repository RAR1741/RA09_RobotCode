#ifndef TURRET_H__
#define TURRET_H__

#include "WPILib.h"
#include "Target.h"
// #include "Robot.cpp"

class Turret
{
public:
	Turret();
	~Turret();
	
	void TurretControl();
	
	TrackingThreshold PrimaryThreshold() const { return td1; };
	TrackingThreshold SecondaryThreshold() const { return td2; };
	bool TargetInSight() const { return tracking; };
	double GetTarget_X(); // HaHa Patrick! Added a function to YOUR code! How does THAT make you feel?
	// Am I a bad person? >:)
	// Yes. --PF
	
	
	ParticleAnalysisReport TargetData() const { return par; }

	
private:
	//PurpleMonkeys *theRobot;
	
	bool tracking;
	TrackingThreshold td1, td2;		// color thresholds	
	TrackingThresholdRGB custom1, custom2; // Custom color thresholds
	
	ParticleAnalysisReport par;		// particle analysis report
};

#endif
