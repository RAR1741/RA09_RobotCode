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
private:
	
};
#endif
