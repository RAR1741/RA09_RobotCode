#ifndef AUTONOMOUS__H
#define AUTONOMOUS__H

// This excludes any functional code from the robot code, allowing
// this branch to compile. Sorry Mr. Miller, but the code has multi-line
// comments.
#define EXCLUDE_AUTO	1

#include "wpilib.h"

#if !EXCLUDE_AUTO
#include "TrackAPI.h" // TrackAPI depends on wpilib.h DON'T ERASE THAT INCLUDE!
// #include "Robot.h"
#endif





class Autonomous
{
	//Autonomous(PurpleMonkeys *);
public:
	Autonomous();
	~Autonomous();
	void SetControls(RobotDrive *);
	void Init();
	void Periodic();
	void Continuous();
	void IdentifyFriendOrFoe();
private:
	void AutoProgram0();
	void AutoProgram1();
	void AutoProgram2();
	////////////////////////////////////////////////////////
	// STATE
	UINT32 Stage;
	UINT32 StageTickCount;
	UINT32 Program;
	RobotDrive *TheRobot;
	UINT32 Program0Stages[9];
	UINT32 Program0StageCounts[9];

#if !EXCLUDE_AUTO
	TrackingThreshold td1, td2;		// color thresholds			
	ParticleAnalysisReport par;		// particle analysis report
#endif
};
#endif
