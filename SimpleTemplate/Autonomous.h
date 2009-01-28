#ifndef AUTONOMOUS__H
#define AUTONOMOUS__H
//#include "TrackAPI.h"
//#include "Robot.h"

class Autonomous
{
	//Autonomous(PurpleMonkeys *);
public:
	Autonomous();
	~Autonomous();
	void Init();
	void Periodic();
	void Continuous();
};
#endif
