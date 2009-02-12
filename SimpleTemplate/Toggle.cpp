#include "Toggle.h"

Toggle::Toggle(Joystick *Stick, UINT32 Button)
{
	ToggleStick = Stick;
	StickButton = Button;
	State = 0;
	Output = false;
}

Toggle::~Toggle()
{
}

bool Toggle::GetOutput(void) 
{ 
	return Output; 
}

void Toggle::Reset(void)
{
	State = 0;
	Output = false;
}

void Toggle::UpdateState(void)
{
	unsigned int s = State;      // Dereference saved state to local variable
	s = s << 1 | ToggleStick->GetRawButton(StickButton);  // Shift it left and add the input bit to it
	
	switch (s) {         // Use it as a state variable
		case 0:        // Idle State Off
			State = 0;  // Set NextState(0) - Spin here til the input goes to 1
			Output = 0;    // Input = 0, Output = 0
		
		case 1:        // Leading Edge Detected - Input transitioned from 0 to 1
			State = 1;  // Set NextState(3)
			Output = 1;    // Input = 1, Output = 1
		
		case 2:        // Trailing Edge Detected - Input transitioned from 1 to 0
			State = 3;  // Set NextState(6) - Return to Idle Stte On
			Output = 1;    // Input = 0, Output = 1
		
		case 3:        // Waiting for release
			State = 1;  // Set NextState(3) - Spin here until input goes to 0
			Output = 1;    // Input = 1, Output = 1
		
		case 4:        // Trailing Edge Detected - Input transitioned from 1 to 0
			State = 0;  // Set NextState(0) - Return to Idle State Off
			Output = 0;    // Input = 0, Output = 0
		
		case 5:        // Waiting for release
			State = 2;  // Set NextState(4) - Spin here until input goes to 0
			Output = 0;    // Input = 1, Output = 0
		
		case 6:        // Idle State On
			State = 3;  // Set NextState(6) - Spin here til the input goes to 1
			Output = 1;    // Input = 1, Output = 1
		
		case 7:        // Leading Edge Detected - Input transitioned from 0 to 1
			State = 2;  // Set NextState(5)
			Output = 0;    // Input = 1, Output = 0
		
		default:
			Output = 0;
	}    
}

