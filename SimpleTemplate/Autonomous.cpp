
#include "Autonomous.h"
#include "DriverStationLCD.h"

#define ACTION_STOP 0
#define ACTION_STRAIGHT_QUARTER 1
#define ACTION_STRAIGHT_HALF 2
#define ACTION_STRAIGHT_FULL 3
#define ACTION_LEFT_QUARTER 4 
#define ACTION_LEFT_HALF 5
#define ACTION_LEFT_FULL 6
#define ACTION_RIGHT_QUARTER 7
#define ACTION_RIGHT_HALF 8
#define ACTION_RIGHT_FULL 9
#define ACTION_ARC_RIGHT 10
#define ACTION_ARC_LEFT 11
#define ACTION_REVERSE_HALF 12
#define ACTION_REVERSE_FULL 13
#define ACTION_ARC_RIGHT_SHARP 14
#define ACTION_ARC_RIGHT_SHARP_QUARTER 15
#define ACTION_ARC_RIGHT_SHARP_HALF 16
#define ACTION_ARC_RIGHT_QUARTER 17
#define ACTION_ARC_RIGHT_HALF 18
#define ACTION_ARC_LEFT_QUARTER 19
#define ACTION_ARC_LEFT_HALF 20

#if !EXCLUDE_AUTO
#include "Target.h"
#endif

//Autonomous::Autonomous(PurpleMonkeys * Robot)
Autonomous::Autonomous() {

}

Autonomous::~Autonomous() {
}

void Autonomous::SetControls(RobotDrive *myRobot){
	TheRobot = myRobot;
}


void Autonomous::AutonomousStages()
{
	
	// Far corner of field
	ProgramStages[0][0] = ACTION_ARC_RIGHT_QUARTER;
	StageDuration[0][0] = 0.5;
	ProgramStages[0][1] = ACTION_ARC_RIGHT_HALF;
	StageDuration[0][1] = 0.5;
	ProgramStages[0][2] = ACTION_ARC_RIGHT;
	StageDuration[0][2] = 3.0;
	ProgramStages[0][3] = ACTION_STRAIGHT_HALF;
	StageDuration[0][3] = 6.0;
	ProgramStages[0][4] = ACTION_REVERSE_HALF;
	StageDuration[0][4] = 4;
	ProgramStages[0][5] = ACTION_STRAIGHT_HALF;
	StageDuration[0][5] = 1;
//	ProgramStages[1][3] = ACTION_REVERSE_HALF;
//	StageDuration[1][3] = 2;
//	ProgramStages[1][4] = ACTION_STRAIGHT_HALF;
//	StageDuration[1][4] = 2;
//	ProgramStages[1][5] = ACTION_REVERSE_HALF;
//	StageDuration[1][5] = 1.5;
    
	// Closest corner to driver
	ProgramStages[1][0] = ACTION_ARC_LEFT_QUARTER;
	StageDuration[1][0] = 0.5;
	ProgramStages[1][1] = ACTION_ARC_LEFT_HALF;
	StageDuration[1][1] = 0.5;
	ProgramStages[1][2] = ACTION_ARC_LEFT;
	StageDuration[1][2] = 3.0;
	ProgramStages[1][3] = ACTION_STRAIGHT_HALF;
	StageDuration[1][3] = 6.0;
	ProgramStages[1][4] = ACTION_REVERSE_HALF;
	StageDuration[1][4] = 4;
	ProgramStages[1][5] = ACTION_STRAIGHT_HALF;
	StageDuration[1][5] = 1;
//	ProgramStages[1][3] = ACTION_REVERSE_HALF;
//	StageDuration[1][3] = 2;
//	ProgramStages[1][4] = ACTION_STRAIGHT_HALF;
//	StageDuration[1][4] = 2;
//	ProgramStages[1][5] = ACTION_REVERSE_HALF;
//	StageDuration[1][5] = 1.5;
	
	// Outpost in middle of field.
	ProgramStages[2][0] = ACTION_ARC_RIGHT_SHARP_QUARTER;
	StageDuration[2][0] = 0.5;
	ProgramStages[2][1] = ACTION_ARC_RIGHT_SHARP_HALF;
	StageDuration[2][1] = 0.5;
	ProgramStages[2][2] = ACTION_ARC_RIGHT_SHARP;
	StageDuration[2][2] = 3.0;
	ProgramStages[2][3] = ACTION_STRAIGHT_HALF;
	StageDuration[2][3] = 6.0;
	ProgramStages[2][4] = ACTION_REVERSE_HALF;
	StageDuration[2][4] = 4;
	ProgramStages[2][5] = ACTION_STRAIGHT_HALF;
	StageDuration[2][5] = 1;
//	ProgramStages[1][3] = ACTION_REVERSE_HALF;
//	StageDuration[1][3] = 2;
//	ProgramStages[1][4] = ACTION_STRAIGHT_HALF;
//	StageDuration[1][4] = 2;
//	ProgramStages[1][5] = ACTION_REVERSE_HALF;
//	StageDuration[1][5] = 1.5;
}

void Autonomous::AutonomousTestStages()
{
	ProgramStages[0][0] = ACTION_LEFT_FULL;
	StageDuration[0][0] = 100.0;

    
	ProgramStages[1][0] = ACTION_STRAIGHT_FULL;
	StageDuration[1][0] = 100.0;

	
	ProgramStages[2][0] = ACTION_RIGHT_FULL;
	StageDuration[2][0] = 100.0;
}

void Autonomous::Init() {
	Stage = 0;
	CurrentStageDuration = 0;
	Program = 0;
	
	
	// This sets all of the array elements to ACTION_STOP by default,
	// and the Stage duration to 1000. This eliminates the potential for
	// a bad autonomous program, but you also do not need to add the stop
	// part to the end of your programs because they are added by this loop.
	
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 30; j++){
			ProgramStages[i][j] = ACTION_STOP;
			StageDuration[i][j] = 1000;
		}
	}
	
	AutonomousStages();
	//AutonomousTestStages();

#if !EXCLUDE_AUTO
	/* image data for tracking - override default parameters if needed */
	/* recommend making PINK the first color because GREEN is more 
	 * subsceptible to hue variations due to lighting type so may
	 * result in false positives */
	// PINK
	sprintf(td1.name, "PINK");
	td1.hue.minValue = 220;
	td1.hue.maxValue = 255;
	td1.saturation.minValue = 75;
	td1.saturation.maxValue = 255;
	td1.luminance.minValue = 85;
	td1.luminance.maxValue = 255;
	// GREEN
	sprintf(td2.name, "GREEN");
	td2.hue.minValue = 55;
	td2.hue.maxValue = 125;
	td2.saturation.minValue = 58;
	td2.saturation.maxValue = 255;
	td2.luminance.minValue = 92;
	td2.luminance.maxValue = 255;
	/* set up debug output: 
	 * DEBUG_OFF, DEBUG_MOSTLY_OFF, DEBUG_SCREEN_ONLY, DEBUG_FILE_ONLY, DEBUG_SCREEN_AND_FILE 
	 */
	SetDebugFlag(DEBUG_SCREEN_ONLY);

	/* start the CameraTask	 */
	if (StartCameraTask(framesPerSecond, 0, k320x240, ROT_180) == -1) {
		DPRINTF(LOG_ERROR,
				"Failed to spawn camera task; exiting. Error code %s",
				GetVisionErrorText(GetLastVisionError()) );
	}
	/* allow writing to vxWorks target */
	Priv_SetWriteFileAllowed(1);
#endif
}

void Autonomous::Continuous() {

}

#if !EXCLUDE_AUTO
#error "This project isn't building because this code was not refactored from TwoColorDemo."
/**
	 * Set servo positions (0.0 to 1.0) translated from normalized values (-1.0 to 1.0). 
	 * 
	 * @param normalizedHorizontal Pan Position from -1.0 to 1.0.
	 * @param normalizedVertical Tilt Position from -1.0 to 1.0.
	 */
	void setServoPositions(float normalizedHorizontal, float normalizedVertical)	{

		float servoH = NormalizeToRange(normalizedHorizontal);
		float servoV = NormalizeToRange(normalizedVertical);
		
		float currentH = horizontalServo->Get();		
		float currentV = verticalServo->Get();
		
		/* make sure the movement isn't too small */
		if ( fabs(servoH - currentH) > servoDeadband ) {
			horizontalServo->Set( servoH );
			/* save new normalized horizontal position */
			horizontalPosition = RangeToNormalized(servoH, 1);
		}
		if ( fabs(servoV - currentV) > servoDeadband ) {
			verticalServo->Set( servoV );
			verticalPosition = RangeToNormalized(servoV, 1);
		}
	}	

	/**
	 * Adjust servo positions (0.0 to 1.0) translated from normalized values (-1.0 to 1.0). 
	 * 
	 * @param normalizedHorizontal Pan adjustment from -1.0 to 1.0.
	 * @param normalizedVertical Tilt adjustment from -1.0 to 1.0.
	 */
	void adjustServoPositions(float normDeltaHorizontal, float normDeltaVertical)	{
						
		/* adjust for the fact that servo overshoots based on image input */
		normDeltaHorizontal /= 8.0;
		normDeltaVertical /= 4.0;
		
		/* compute horizontal goal */
		float currentH = horizontalServo->Get();
		float normCurrentH = RangeToNormalized(currentH, 1);
		float normDestH = normCurrentH + normDeltaHorizontal;	
		/* narrow range keep servo from goi	ng too far */
		if (normDestH > 1.0) normDestH = 1.0;
		if (normDestH < -1.0) normDestH = -1.0;			
		/* convert input to servo range */
		float servoH = NormalizeToRange(normDestH);

		/* compute vertical goal */
		float currentV = verticalServo->Get();
		float normCurrentV = RangeToNormalized(currentV, 1);
		float normDestV = normCurrentV + normDeltaVertical;	
		if (normDestV > 1.0) normDestV = 1.0;
		if (normDestV < -1.0) normDestV = -1.0;
		/* convert input to servo range */
		float servoV = NormalizeToRange(normDestV, 0.2, 0.8);

		/* make sure the movement isn't too small */
		if ( fabs(currentH-servoH) > servoDeadband ) {
			horizontalServo->Set( servoH );		
			/* save new normalized horizontal position */
			horizontalPosition = RangeToNormalized(servoH, 1);
		}			
		if ( fabs(currentV-servoV) > servoDeadband ) {
			verticalServo->Set( servoV );
			verticalPosition = RangeToNormalized(servoV, 1);
		}
	}
#endif
void Autonomous::Periodic() {
	switch (ProgramStages[Program][Stage]){
		case ACTION_STOP:
			TheRobot->Drive(0.0, 0.0);
			break;
			
		case ACTION_STRAIGHT_QUARTER:
			TheRobot->Drive(0.25, 0.0);
			break;
			
		case ACTION_STRAIGHT_HALF:
			TheRobot->Drive(0.5, 0.0);
			break;
			
		case ACTION_STRAIGHT_FULL:
			TheRobot->Drive(1.0, 0.0);
			break;
			
		case ACTION_LEFT_QUARTER: 
			TheRobot->Drive(0.5, -0.25);
			break;
			
		case ACTION_LEFT_HALF:
			//TheRobot->Drive(0.5, -0.5);
			TheRobot->Drive(0.5, -0.75);
			break;
						
		case ACTION_LEFT_FULL:
			TheRobot->Drive(0.5, -1.0);
			break;
			
		case ACTION_RIGHT_QUARTER:
			TheRobot->Drive(0.5, 0.25);
			break;
			
		case ACTION_RIGHT_HALF:
			TheRobot->Drive(0.5, 0.5);
			break;
			
		case ACTION_RIGHT_FULL:
			TheRobot->Drive(0.5, 1.0);
			break;

		case ACTION_ARC_RIGHT:
			TheRobot->Drive(0.9, .1);
			break;
			
		case ACTION_ARC_LEFT:
			TheRobot->Drive(0.9, -.1);
			break;
			
		case ACTION_ARC_LEFT_QUARTER:
			TheRobot->Drive(0.25, -.1);
			break;
			
		case ACTION_ARC_LEFT_HALF:
			TheRobot->Drive(0.5, -.1);
			break;
			
		case ACTION_REVERSE_HALF:
			TheRobot->Drive(-0.5, 0.0);
			break;
			
		case ACTION_REVERSE_FULL:
			TheRobot->Drive(-1.0, 0.0);
			break;
			
		case ACTION_ARC_RIGHT_SHARP:
			TheRobot->Drive(0.9, .2);
			break;

		case ACTION_ARC_RIGHT_SHARP_QUARTER:
			TheRobot->Drive(0.25, .2);
			break;

		case ACTION_ARC_RIGHT_SHARP_HALF:
			TheRobot->Drive(0.5, .2);
			break;

		case ACTION_ARC_RIGHT_QUARTER:
			TheRobot->Drive(0.25, .1);
			break;

		case ACTION_ARC_RIGHT_HALF:
			TheRobot->Drive(0.5, .1);
			break;

		default:
			TheRobot->Drive(0.0, 0.0);
			break;
	}
	StageTimer.Start();
	
	DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
    dsLCD->Printf(DriverStationLCD::kUser_Line3, 12, "AS:%1d %3.1f", Stage, StageTimer.Get());
    dsLCD->UpdateLCD();
	
	if (StageTimer.Get() >= StageDuration[Program][Stage])
	{
		Stage++;
		StageTimer.Reset();
	}
	
#if !EXCLUDE_AUTO
	
	if (FindTwoColors(td1, td2, ABOVE, &par) ) {
		//PrintReport(&par);
		foundColor = true;
		// reset pan		
		panIncrement = 0;
		if (par.imageTimestamp == savedImageTimestamp) {
			// This image has been processed already, 
			// so don't do anything for this loop 
			staleImage = true;
			DPRINTF(LOG_DEBUG, "STALE IMAGE");

		} else {
			// The target was recognized
			// save the timestamp
			staleImage = false;
			savedImageTimestamp = par.imageTimestamp;
			DPRINTF(LOG_DEBUG, "image timetamp: %lf", savedImageTimestamp);

			// Here is where your game-specific code goes
			// when you recognize the target

			// get center of target 

			// TODO: use par.average_mass_x_normalized and
			// par.average_mass_y_normalized after updated WPILib is distributed
			horizontalDestination = par.center_mass_x_normalized;
			verticalDestination = par.center_mass_y_normalized;
		}
	} else { // need to pan 
		foundColor = false;
	}
	if (foundColor && !staleImage) {
		/* Move the servo a bit each loop toward the destination.
		 * Alternative ways to task servos are to move immediately vs.
		 * incrementally toward the final destination. Incremental method
		 * reduces the need for calibration of the servo movement while
		 * moving toward the target.
		 */
		incrementH = horizontalDestination - horizontalPosition;
		// you may need to reverse this based on your vertical servo installation
		//incrementV = verticalPosition - verticalDestination;
		incrementV = verticalDestination - verticalPosition;
		adjustServoPositions(incrementH, incrementV);

		ShowActivity("** %s & %s found: Servo: x: %f  y: %f ** ", td1.name,
				td2.name, horizontalDestination, verticalDestination);

	} else { //if (!staleImage) {  // new image, but didn't find two colors

		// adjust sine wave for panning based on last movement direction
		if (horizontalDestination > 0.0) {
			sinStart = PI/2.0;
		} else {
			sinStart = -PI/2.0;
		}

		/* pan to find color after a short wait to settle servos
		 * panning must start directly after panInit or timing will be off */
		if (panIncrement == 3) {
			panInit(8.0); // number of seconds for a pan
		} else if (panIncrement > 3) {
			panForTarget(horizontalServo, sinStart);

			/* Vertical action: In case the vertical servo is pointed off center,
			 * center the vertical after several loops searching */
			if (panIncrement == 20) {
				verticalServo->Set( 0.5);
			}
		}
		panIncrement++;

		ShowActivity(
				"** %s and %s not found                                    ",
				td1.name, td2.name);
	} // end if found color

	// sleep to keep loop at constant rate
	// this helps keep pan consistant
	// elapsed time can vary significantly due to debug printout
	currentTime = GetTime();
	lastTime = currentTime;
	if (loopTime > ElapsedTime(lastTime) ) {
		Wait(loopTime - ElapsedTime(lastTime) ); // seconds
	}
#endif
}

void Autonomous::SetProgramNumber(UINT32 ProgramNum){
	Program = ProgramNum;
}

UINT32 Autonomous::GetProgramNumber(void){
	return Program;
}

#if !EXCLUDE_AUTO
void Autonomous::IdentifyFriendOrFoe() {
	// TODO: Put some actual C++ code in here
	search;
	FindColor()
}

#endif
