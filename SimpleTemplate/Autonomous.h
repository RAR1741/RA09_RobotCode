#ifndef AUTONOMOUS__H
#define AUTONOMOUS__H

// This excludes any functional code from the robot code, allowing
// this branch to compile. Sorry Mr. Miller, but the code has multi-line
// comments.
#define EXCLUDE_AUTO	1

#include "wpilib.h"
#include "Timer.h"

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
	void SetProgramNumber(UINT32);
	UINT32 GetProgramNumber(void);
private:
	////////////////////////////////////////////////////////
	// STATE
	UINT32 Stage;
	UINT32 CurrentStageDuration;
	UINT32 Program;
	RobotDrive *TheRobot;
	UINT32 ProgramStages[3][30];
	double StageDuration[3][30];
	Timer StageTimer;

#if !EXCLUDE_AUTO
	TrackingThreshold td1, td2;		// color thresholds			
	ParticleAnalysisReport par;		// particle analysis report
#endif
};
#endif
