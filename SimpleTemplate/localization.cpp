

/******* Warning: This section is a few bytes from full in the idata memory section. *****
******* Adding more variables here will likely cause link errors *****/

#include <stdio.h>
#include <math.h>
#include "localization.h"
#include <string.h>

// Start of Path Following Routines


// Constants

#define RobotThaddeus2008
#ifdef RobotThaddeus2008
//	float Inc2InchMultiplier = 0.813958096611901;  // Inc2Inch - This converts encoder counts to inches - 22 teeth
	float Inc2InchMultiplier = 0.440570969753426;  // Inc2Inch - This converts encoder counts to inches - 42 teeth
	float RobotWheelBase = 28.0;  // Distance in inches center line to centerline between drive wheels on robot - 2008 machine
#endif

#ifdef RobotMantas2007
	float Inc2InchMultiplier = 0.897597901025655;  // This converts Encoder counts to inches
	float RobotWheelBase = 15.0;  // Distance in inches center line to centerline between drive wheels on robot - 2007 machine
#endif

	float Rad2Deg = 180/3.14159265358979323846264338327950288419716939937510;
	float DeltaTime = 0.026;  // Time between passed through this code

extern	int ThrottleYAxisJoystick;
extern	int SteerXAxisJoystick;

// Start of Global variables

	// Output from CalcInches
	int CurrentEcountLeft = 0;
	int CurrentEcountRight = 0;
	int PreviousEcountLeft = 0;
	int PreviousEcountRight = 0;
	int EcntLtDelta = 0;
	int EcntRtDelta = 0;
	float EcntLtDeltaInches = 0.0;  // This is the left output value needed in the OdometryMath routine
	float EcntRtDeltaInches = 0.0;  // This is the right output value needed in the OdometryMath routine
	float SpeedFPSLeft = 0.0;
	float SpeedFPSRight = 0.0;

	// Output from OdometryMath
	float xi = 0.0;         // X position in inches - Formula 1.7a
	float yi = 0.0;         // Y position in inches - Formula 1.7b
	float Delta_Ui = 0.0;   // Incremental linear displacement - Formula 1.4
	float Delta_Oi = 0.0;   // Incremental change of orientation - Formula 1.5
	float Oi = 0.0;         // Relative Orientation Oi - In Radians - Formula 1.6
	float EncHding = 0.0;   // Heading in degrees calculated from the encoder data - (Oi converted to degrees)



	struct WayPointInt {
		int	X;
		int	Y;
	};

	struct WayPointReal {
		float X;
		float Y;
	};

	struct WayPointLong {
		long X;
		long Y;
	};
	
	struct YawRotationTransformMatrix {
		float rmA11;  // X value from WayPoint Unit Vector
		float rmA12;  // Y value from WayPoint Unit Vector
		float rmA13;  // Zero
		float rmA21;  // Sign inverted Y value from WayPoint Unit Vector
		float rmA22;  // X value from WayPoint Unit Vector
		float rmA23;  // Zero
		float rmA31;  // Zero
		float rmA32;  // Zero
		float rmA33;  // One
	};

	struct PathMathIO {
		struct WayPointInt CurrentLocation;  // Current location in the arena - Input for ActiveLocation calculations
		struct WayPointInt MapSegmentStart;  // Start point of the current map segment - Input for ActiveRouteSegment calculations
		struct WayPointInt MapSegmentEnd;    // End point of the current map segment - Input for ActiveRouteSegment calculations

	// Start of ActiveRouteSegment variables
		int HotMapSegment;                   // Path number of the current or live route map path segment, zero based.
		struct WayPointLong DeltaWayPoint;
		struct WayPointInt dwpSignMult;
		struct WayPointLong DeltaSquared;
		long CenterLineSquared;
		struct WayPointReal WPUnitVector;   // Unit vector of the current map segment
		int TargetHeading;                  // Heading angle for the current map segment
		struct YawRotationTransformMatrix YawTransform;  // The rotation matrix to move from arena cooridinate system to the
		int SegmentTerminator;              // Value used to decide when to move to the next map segment
	// End of ActiveRouteSegment variables

	// Start of Active Location variables
		struct WayPointInt vWI;
		struct WayPointReal vCLI_real;   // The answer as a real number
		struct WayPointInt vCLI_int;     // The answer rounded to the nearest integer
		int SegmentRemaining;            // Travel distance remaining in the HotMapSegment
	// End of Active Location variables

	};

	
	PathMathIO PM;
	
// End of Global variables

void CalcInches (void)
{
	PreviousEcountLeft = CurrentEcountLeft;     // Move last current to previous
	PreviousEcountRight = CurrentEcountRight;   // Move last curretn to previous

	// These are just temporary values. There were calls to GetLeftGearCount and whatever here, but they don't work here so...
	CurrentEcountLeft = 1;  // Get new current left encoder count from function
	CurrentEcountRight = 1;  // Get new current right encoder count from function

	EcntLtDelta = CurrentEcountLeft - PreviousEcountLeft;  // Calculate the left delta in counts
	EcntRtDelta = CurrentEcountRight - PreviousEcountRight;  // Calculate the right delta in counts

	EcntLtDeltaInches = EcntLtDelta * Inc2InchMultiplier;  // Convert the left delta count to inches
	EcntRtDeltaInches = EcntRtDelta * Inc2InchMultiplier;  // Convert the right deta count to inches

	SpeedFPSLeft  = (EcntLtDeltaInches / DeltaTime); // Inches for now / 12);  // Left speed in Feet Per Second
	SpeedFPSRight = (EcntRtDeltaInches / DeltaTime); // Inches for now / 12);  // Right speed in Feet Per Second

}

void OdometryMath (void)  //  Formula numbers reference page 20 in "Where Am I?" document
{
	// Calculate the Incremental Linear displacement Delta_Ui - Formula 1.4
	Delta_Ui = (EcntRtDeltaInches + EcntLtDeltaInches) / 2;

	// Calculate the incremental change of orientation Delta_Oi - Formula 1.5
	Delta_Oi = (EcntRtDeltaInches - EcntLtDeltaInches) / RobotWheelBase;

	// Calculate the Relative Orientation Oi - in radians - Fourmula 1.6
	Oi = Oi + Delta_Oi;

	// Calculate the heading in degrees
	EncHding = Oi * Rad2Deg;

	// Calculate the X position in inches xi - Formula 1.7a
	xi = xi + Delta_Ui * cos(Oi);

	// Calculate the Y position in inches yi - Formula 1.7b
	yi = yi + Delta_Ui * sin(Oi);
}




extern int RouteMap[rmMaxPoints][3];

extern int RobotStartLocP1[rslMaxPoints][2];

extern int RobotStartLocP2[rslMaxPoints][2];


void TeamSideRoute(int RouteCommand)
{
		RouteMap[2][0] = -96;
		RouteMap[3][0] = 0;
		RouteMap[4][0] = 96;

		switch (RouteCommand)
		{
				case 1:
				RouteMap[2][1] = -36;
				RouteMap[3][1] = -36;
				RouteMap[4][1] = -36;
				break;


				case 2:
				RouteMap[2][1] = -78;
				RouteMap[3][1] = -78;
				RouteMap[4][1] = -78;
				break;


				case 3:
				RouteMap[2][1] = -120;
				RouteMap[3][1] = -120;
				RouteMap[4][1] = -120;
				break;
		}
}

void OppositeSideRoute(int RouteCommand)
{
			RouteMap[7][0] = 96;
			RouteMap[8][0] = 0;
			RouteMap[9][0] = -96;

			switch (RouteCommand)
			{
				case 1:
				RouteMap[7][1] = 36;
				RouteMap[8][1] = 36;
				RouteMap[9][1] = 36;
				break;


				case 2:
				RouteMap[7][1] = 78;
				RouteMap[8][1] = 78;
				RouteMap[9][1] = 78;
				break;


				case 3:
				RouteMap[7][1] = 120;
				RouteMap[8][1] = 120;
				RouteMap[9][1] = 120;
				break;
		}
}




//struct PathMathIO PM = {{96,120},{228,78},{96,36},{0},{0,0},{0,0},{0,0},{0},{0.0,0.0},{0},{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0},{0},{0,0},{0.0,0.0},{0,0},{0}};

void SetStartPosition ( int spn)
{
	RouteMap[0][rmX] = RobotStartLocP1[spn][rslX];  // Move starting location to the route map
	RouteMap[0][rmY] = RobotStartLocP1[spn][rslY];

	RouteMap[1][rmX] = RobotStartLocP2[spn][rslX];  // Move starting location to the route map
	RouteMap[1][rmY] = RobotStartLocP2[spn][rslY];

	xi = RobotStartLocP1[spn][rslX];  // Move starting locaion to the X and Y positions for the OdometryMath routines
	yi = RobotStartLocP1[spn][rslY];
}

void DefineActiveLocation (void)
{
	// Calculate the X portion of the WI vector
	PM.vWI.X = PM.CurrentLocation.X - PM.MapSegmentStart.X;

	// Calculate the Y portion of the WI vector
	PM.vWI.Y = PM.CurrentLocation.Y - PM.MapSegmentStart.Y;

	// Calculate the X portion of the CLI vector
	PM.vCLI_real.X = (PM.vWI.X * PM.YawTransform.rmA11) + (PM.vWI.Y * PM.YawTransform.rmA12);
	PM.vCLI_int.X = (int)floor(PM.vCLI_real.X +.5);

	// Calculate the Y portion of the CLI vector
	PM.vCLI_real.Y = (PM.vWI.X * PM.YawTransform.rmA21) + (PM.vWI.Y * PM.YawTransform.rmA22);
	PM.vCLI_int.Y = (int)floor(PM.vCLI_real.Y +.5);

	// Calculate the SegmentRemaining distance
	PM.SegmentRemaining = PM.SegmentTerminator - PM.vCLI_int.X;
}

int ReturnSignMultiplier ( int InValue)
{
	int s = 1;
	if (InValue < 0) s = -1;
	return s;
}

void DefineActiveRouteSegment (void)
{
	// Set the Hot Segment
	PM.MapSegmentStart.X = RouteMap[PM.HotMapSegment][rmX];
	PM.MapSegmentStart.Y = RouteMap[PM.HotMapSegment][rmY];
	PM.MapSegmentEnd.X = RouteMap[PM.HotMapSegment+1][rmX];
	PM.MapSegmentEnd.Y = RouteMap[PM.HotMapSegment+1][rmY];

	// Calculate the Delta X value and square it
	PM.DeltaWayPoint.X = PM.MapSegmentEnd.X - PM.MapSegmentStart.X;
	PM.DeltaSquared.X = PM.DeltaWayPoint.X * PM.DeltaWayPoint.X;

	// Determine sign of Delta X and save the multiplier
	PM.dwpSignMult.X = ReturnSignMultiplier (PM.DeltaWayPoint.X);

	// Calculate the Delta Y value and square it
	PM.DeltaWayPoint.Y = PM.MapSegmentEnd.Y - PM.MapSegmentStart.Y;
	PM.DeltaSquared.Y = PM.DeltaWayPoint.Y * PM.DeltaWayPoint.Y;

	// Determine sign of Delta Y and save the multiplier
	PM.dwpSignMult.Y = ReturnSignMultiplier (PM.DeltaWayPoint.Y);

	// Add the squared delta Y + X and save it
	PM.CenterLineSquared = PM.DeltaSquared.X + PM.DeltaSquared.Y;

	// Calculate the X portion of the unit vector
	PM.WPUnitVector.X = (sqrt(PM.CenterLineSquared - PM.DeltaSquared.Y) / sqrt(PM.CenterLineSquared)) * PM.dwpSignMult.X;

	// Calculate the Y portion of the unit vector
	PM.WPUnitVector.Y = (sqrt(PM.CenterLineSquared - PM.DeltaSquared.X) / sqrt(PM.CenterLineSquared)) * PM.dwpSignMult.Y;

	// Calculate the Target Heading Angle of this RouteMap segment in degrees
	PM.TargetHeading = (int)(asin(PM.WPUnitVector.Y) * Rad2Deg);

	// Populate the YawRotationTransformMatrix
	PM.YawTransform.rmA11 = PM.WPUnitVector.X;
	PM.YawTransform.rmA12 = PM.WPUnitVector.Y;
	PM.YawTransform.rmA21 = PM.WPUnitVector.Y * -1;
	PM.YawTransform.rmA22 = PM.WPUnitVector.X;

	// Define the segment terminator
	PM.SegmentTerminator = (int)floor(sqrt(PM.CenterLineSquared) + 0.5);
}

int HotSegmentEndThreshold = 0;  // Sets distance before next segment when code starts looking at next segment

void IntializePathMath (void)
{
	PM.HotMapSegment = 0;  // Set the Segment Number to the start
	DefineActiveRouteSegment();  // Set MapSegmentStart and MapSegmetnEnd to the starting values from the map table and calculate setment varialbes
}

void PathMath (void)  // Note: This code is drastically different than ASM code due to program flow stuff - Test carefully!!!
{
	PM.CurrentLocation.X = (int)floor(xi + 0.5);  // Get current location from odometry code and move it
	PM.CurrentLocation.Y = (int)floor(yi + 0.5);  // to the CurrentLocation in the PathMath data structure
	while (-1) {
		DefineActiveLocation();
		if (PM.SegmentRemaining > HotSegmentEndThreshold) return;
		++PM.HotMapSegment;
		if (PM.HotMapSegment >= rmMaxPoints-1) return;
		DefineActiveRouteSegment();
		}
}

#define	FloatScale 100  // This is an int since it is written with no terminal charater (K&R pg 37)

int f2i2 (float InValue)
{
	return int(InValue * FloatScale);	
}

int f2i (float InValue, int Scale)
{
	return int(InValue * Scale);	
}

void LogPathMath(void)
{
//	OSCILLOSCOPE = 1;  // For testing timing of this routine with O scope - Output on DO13
// Start of CalcInches variables
	printf ("%d %d ", CurrentEcountLeft, CurrentEcountRight);
//	printf ("%d %d ", PreviousEcountLeft, PreviousEcountRight);
//	printf ("%d %d ", EcntLtDelta, EcntRtDelta);
//	printf ("%d %d ", f2i2(EcntLtDeltaInches), f2i2(EcntRtDeltaInches));
//	printf ("%d %d ", f2i2(SpeedFPSLeft), f2i2(SpeedFPSRight));
// End of CalcInches variables

// Start of OdometryMath variables
//	printf ("%d ", f2i(Delta_Ui, 100));
//	printf ("%d ", f2i(Delta_Oi, 100));
//	printf ("%d ", f2i(Oi, 100));
	printf ("%d ", f2i(xi, 100));
	printf ("%d ", f2i(yi, 100));
	printf ("%d ", f2i(EncHding, 10));
// End of OdometryMath variables

// Start of Input variables
//	printf("%d %d ", PM.CurrentLocation.X, PM.CurrentLocation.Y);
//	printf("%d %d ", PM.MapSegmentStart.X, PM.MapSegmentStart.Y);
//	printf("%d %d ", PM.MapSegmentEnd.X, PM.MapSegmentEnd.Y);
// End of Input variables

// Start of ActiveRouteSegment variables
	printf("%d ", PM.HotMapSegment);
// Don't show correct because of LONG	printf("PM.DeltaWayPoint.X %d PM.DeltaWayPoint.Y %d ", PM.DeltaWayPoint.X, PM.DeltaWayPoint.Y);
//	printf("%d %d ", PM.dwpSignMult.X, PM.dwpSignMult.Y);
// Don't show correct because of LONG	printf("PM.DeltaSquared.X %d PM.DeltaSquared.Y %d ", PM.DeltaSquared.X, PM.DeltaSquared.Y);
// Don't show correct because of LONG	printf("PM.CenterLineSquared %d ", PM.CenterLineSquared);
//	printf("%d %d ", f2i2(PM.WPUnitVector.X), f2i2(PM.WPUnitVector.Y));
//	printf("%d ", PM.TargetHeading);
//	printf("%d %d %d ", f2i2(PM.YawTransform.rmA11), f2i2(PM.YawTransform.rmA12), f2i2(PM.YawTransform.rmA13));
//	printf("%d %d %d ", f2i2(PM.YawTransform.rmA21), f2i2(PM.YawTransform.rmA22), f2i2(PM.YawTransform.rmA23));
//	printf("%d %d %d ", f2i2(PM.YawTransform.rmA31), f2i2(PM.YawTransform.rmA32), f2i2(PM.YawTransform.rmA33));
//	printf("%d ", PM.SegmentTerminator);
// End of ActiveRouteSegment variables

// Start of Active Location variables
//	printf("%d %d ", PM.vWI.X, PM.vWI.Y);
//	printf("%d %d ", f2i2(PM.vCLI_real.X), f2i2(PM.vCLI_real.Y));
//	printf("%d ", PM.vCLI_int.X);
	printf("%d ", PM.vCLI_int.Y);  //  Also known as Lateral Offset
//	printf("%d ", PM.SegmentRemaining);
// End of Active Location variables

// Start of Control Variables
	printf("%d ", ThrottleYAxisJoystick);  // Throttle input from joy stick
	printf("%d ", SteerXAxisJoystick);     // Steering input from joy stick
// End of Control Variables

	printf("\r\n");  // Insert CR LF at end of record
//	OSCILLOSCOPE = 0;  // For testing timing of this routine with O scope - Output on DO13
// End of Active Location variables
}

void LocalizeRobot (void)
{
	CalcInches();    // Determine the delta travel since last time this code ran
	OdometryMath();  // Detemine the X & Y location in inches relative to the arena frame of reference
//	LogPathMath();   // Send localization variables to serial port for inspection or logging
//  printf("PathMath...");
	PathMath();      // Determine distance from the hot segment center line
//  printf("done.\r\n");
	//LogPathMath();   // Send localization variables to serial port for inspection or logging
}

void InitializeLocalizationCode (void)
{
	
	//stdout_serial_port=SERIAL_PORT_TWO;  // Set printf statements to go to the SD card on serial port 2
//	SetStartPosition(1);  // Set start position to first location - for testing - Later will be done by switch settings
//	SetStartPosition(2);  // Set start position to second location - for testing - Later will be done by switch settings
	memset((void*)&PM, 0, sizeof(PM));
	SetStartPosition(3);  // Set start position to third location - for testing - Later will be done by switch settings
	IntializePathMath();
//	LogPathMath();
//	LocalizeRobot();  // This is for testing only
}

// End of Path Following Routines

int GetPM_Y(void)
{
	return PM.vCLI_int.Y;
}
