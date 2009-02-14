
#include "Personality.h"
#include "SerialPort.h"
#include <cstring>
#include <stdio.h>
#include "vxWorks.h" 


Personality::Personality()
{
//	TempSelector = 0;      // Note: these don't really need to be here
//	SubMenuSelector = 0;   // Try deleting them when testing code to validate - HAM

	//fdSerialPort = fopen ("/tyCo/0", "r+");        // Open the serial port for reading and writing
	//fdConfigFile = fopen ("Robality.cfg", "r+");   // Open personality file
}

void Personality::Open(void)
{
	fdSerialPort = fopen ("/tyCo/0", "r+");           // Open the serial port for reading and writing
}

void Personality::Close(void)
{
	fclose (fdSerialPort);                            // Close the port
}

Personality::~Personality()
{
	//fclose (fdConfigFile);   // Close personality file
	//fclose (fdSerialPort);   // Close the port
}

void Personality::SqueekySayHello(void)
{
// ******** Test code to try to send a string to the main console **********
//          Added by HAM 2/6/2009	
	char TestString[22] = "\r\nSqueeky says hello.";  // Test message to send out serial port	
	//fdSerialPort = fopen ("/tyCo/0", "r+");           // Open the serial port for reading and writing
	//fwrite (TestString, sizeof(TestString), 1, fdSerialPort);  // Write the string
	DisplayText(TestString);
	//fclose (fdSerialPort);                            // Close the port
}

void Personality::DisplayText (const char *TxtAdd)
{
  do {fputc(*TxtAdd, fdSerialPort);}  // Send byte to serial port
  while(*TxtAdd++);                   // Until we reach end of string
}

char Personality::GetKeyPress (void)
{
	//int result = fgetc(fdSerialPort);      // If character is ready then return the character
	int result = getc(fdSerialPort);
	if (result == EOF) return 0;
	return result;
}

void Personality::RPTCommandProccessor()
{
	char Command = GetKeyPress();  // Try to get a keypress from keyboard
	if(Command == 0)               // Test for a character present
	  return;                      // If none ready then just return from command processor
	switch(SubMenuSelector)
	{
	 case 0:                                         
	  switch(Command)              // If we did get a keypres then select what to do as result of requested command
	  {
	    case 0xD:   // jmp ApplyAuxCommand  ; 013  0Dh  CR   ^M
	        DisplayText("\r\nSub menu");
	        SubMenuSelector = TempSelector;
		  break;

	    case 0x20:  // ShowText        ; 032  20h  SP
	      DisplayText("\r\nCommand Space");
	      TempSelector = 0;
		  break;

	    case 0x30:  // ShowText        ; 048  30h  0
	      DisplayText("\r\nCommand Zero");
		  break;

	    case 0x31: // Select Steering sub menu  ; 049  31h  1
	      DisplayText("\r\nSteering");
	      TempSelector = 1;
		  break;
	    case 0x32:  // Select Autonomous sub menu ; 050  32h  2
	      DisplayText("\r\nAutonomous");
	      TempSelector = 2;
		  break;
	    case 0x33:  // Select Elevator sub menu ; 051  33h  3
	      DisplayText("\r\nElevator");
	      TempSelector = 3;
		  break;
	    case 0x34:  // ShowText        ; 052  34h  4
	      DisplayText("\r\nCommand Four");
	      TempSelector = 0;
		  break;
	    case 0x35:  // ShowText        ; 053  35h  5
	      DisplayText("\r\nCommand Five");
	      TempSelector = 0;
		  break;
	    case 0x36:  // ShowText        ; 054  36h  6
	      DisplayText("\r\nCommand Six");
	      TempSelector = 0;
		  break;
	    case 0x37:  // ShowText        ; 055  37h  7
	      DisplayText("\r\nCommand Seven");
	      TempSelector = 0;
		  break;
	    case 0x38:  // ShowText        ; 056  38h  8
	      DisplayText("\r\nCommand Eight");
	      TempSelector = 0;
		  break;
	    case 0x39:  // ShowText        ; 057  39h  9
	      DisplayText("\r\nCommand Nine");
	      TempSelector = 0;
		  break;

	    case 0x41:  // ShowText        ; 065  41h  A
	      DisplayText("\r\nCommand A");
	      TempSelector = 0;
		  break;
	    case 0x42:  // ShowText        ; 066  42h  B
	      DisplayText("\r\nCommand B");
	      TempSelector = 0;
		  break;
	    case 0x43:  // ShowText        ; 067  43h  C
	      DisplayText("\r\nCommand C");
	      TempSelector = 0;
		  break;
	    case 0x44:  // ShowText        ; 068  44h  D
	      DisplayText("\r\nCommand D");
	      TempSelector = 0;
		  break;
	    case 0x45:  // ShowText        ; 069  45h  E
	      DisplayText("\r\nCommand E");
	      TempSelector = 0;
		  break;
	    case 0x46:  // ShowText        ; 070  46h  F
	      DisplayText("\r\nCommand F");
	      TempSelector = 0;
		  break;
	    case 0x2D:  // ShowText        ; 045  2Dh  -
	      DisplayText("\r\nCommand -");
	      TempSelector = 0;
		  break;
	    case 0x2E:  // ShowText        ; 046  2Eh  .
	      DisplayText("\r\nCommand .");
	      TempSelector = 0;
		  break;
	    case 0x7F:  // ShowText        ; 127  7Fh  DEL
	      DisplayText("\r\nClear to default");
	      TempSelector = 0;
		  break;
		default:
		  DisplayText("\r\nUnknown Command");  // Print this if none of the above are valid.
		  break;
	   }
		 break;

			case 1:
				switch(Command)
				{
			    case 0xD:
	           DisplayText("\r\nConfirmed entry");
	           SubMenuSelector = 0; //Breaks the sub menu
		      break;
				  case 0x31:
	           DisplayText("\r\nArcade Drive");
		      break;	
	        case 0x32: 
	           DisplayText("\r\nTank Drive");
	        break;
		      default:
		         DisplayText("\r\n");  // Print this if none of the above are valid.
		      break;
	      }
	     break;
	    case 2:
	      switch(Command)
	       { 
			    case 0xD:
	           DisplayText("\r\nConfirmed entry");
	           SubMenuSelector = 0;
		      break;
				  case 0x31: 
	           DisplayText("\r\nGet empty cell");
		      break;	
	        case 0x32: 
	           DisplayText("\r\nCam track shoot");
	        break;
	        case 0x33:
	           DisplayText("\r\nRandomly evade");
	        break;
	        case 0x34:
	           DisplayText("\r\nGet ammo");
	        break;
	        case 0x35:
	           DisplayText("\r\nFind start POS.");
	        break;
		      default:
		         DisplayText("\r\n");  // Print this if none of the above are valid.
		      break;  
	      }
	     break;
	    case 3:
	     switch(Command)
	     {
	      case 0xD:
	           DisplayText("\r\nConfirmed entry");
	           SubMenuSelector = 0;
	      break;
	      case 0x31:
	           DisplayText("\r\nSpeed 1");
	      break;
	      case 0x32:
	           DisplayText("\r\nSpeed 2");
	      break;
		    default:
		         DisplayText("\r\n");  // Print this if none of the above are valid.
		    break;
	     }
	    break;
	  }
}
