#ifndef AUTONOMOUS__H
#define AUTONOMOUS__H
#include "wpilib.h"
#include "TrackAPI.h" // TrackAPI depends on wpilib.h DON'T ERASE THAT INCLUDE!
// #include "Robot.h"

class Autonomous
{
	//Autonomous(PurpleMonkeys *);
public:
	Autonomous();
	~Autonomous();
	void Init();
	void Periodic();
	void Continuous();
	void IdentifyFriendOrFoe();
private:
	TrackingThreshold td1, td2;		// color thresholds			
	ParticleAnalysisReport par;		// particle analysis report
};
#endif
