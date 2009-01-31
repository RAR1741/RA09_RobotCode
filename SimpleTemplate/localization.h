#ifndef LOCALIZATION__H
#define LOCALIZATION__H

#define rmX 0  // Use these in the RouteMap array 2nd index position to select the X or Y value
#define rmY 1
#define rmS 2  // Use this to address the speed variable for a partictular RouteMap segment

#define	rslX 0  // Use these in the RobotStartLoc array 2nd index position to select the X or Y value
#define rslY 1

#define	rmMaxPoints 11  // Number of points in the RouteMap array - make this automatic?
#define rslMaxPoints 4  // Number of points in the RobotStartLoc array - make this automatic?

void LogPathMath(void);
void LocalizeRobot(void);
void InitializeLocalizationCode(void);

//Calculate route points for beginning side. (route map)
void TeamSideRoute(int);
//Calculate route points for opposite side. (route map)
void OppositeSideRoute(int);
void SetStartPosition ( int spn);

int GetPM_Y(void);

#endif
