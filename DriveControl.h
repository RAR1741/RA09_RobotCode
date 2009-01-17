#ifndef DRIVECONTROL__
#define DRIVECONTROL__
#include "WPILib.h"

class DriveControl
{
public:
	DriveControl();
	~DriveControl();
	void Left(float speed);
	void Right(float speed);
	
private:
	UINT8 RawFromFloat(float speed);
};

#endif
