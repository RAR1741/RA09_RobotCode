#ifndef PERSONALITY_H__
#define PERSONALITY_H__

#include "WPILib.h"

class Personality
{
public:
	Personality();
	~Personality();

	void RPTCommandProccessor(void);
	void SqueekySayHello(void);
	
private:
	char SubMenuSelector;  // This state variable keeps track of sub menu from previous command
	char TempSelector;     // Hash register for selection process

	FILE *fdSerialPort;    // File Descriptor for the serial port
	FILE *fdConfigFile;    // Personality variable file
	
	char GetKeyPress (void);
	void DisplayText (const char *TxtAdd);
};

#endif
