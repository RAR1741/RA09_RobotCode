#ifndef PERSONALITY_H__
#define PERSONALITY_H__

#include "WPILib.h"

class Personality
{
public:
	Personality();
	~Personality();
	
	void RPTCommandProccessor(void);
private:
	SerialPort * terminal_port;
	char SubMenuSelector;  // This state variable keeps track of sub menu from previous command
	char TempSelector;     // Hash register for selection process
	char Buffer[128];		// Buffer
	char GetKeyPress (void);
	void DisplayText (const char *TxtAdd);
};

#endif
