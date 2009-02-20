#include "Turret.h"
#include "DriverStationLCD.h"
#include "Mode.h"

#include <cmath>

#define PI 3.14159265358979
#define USE_PID 0
Turret::Turret()
{
	Turret_Pos_Motor = new Jaguar(4,3);
	Clockwise_Limit = new LimitSwitch(6,10);
	CounterClockwise_Limit = new LimitSwitch(6,9);
	Position_Encoder = new AnalogChannel(1,7);
	memset(&par,0,sizeof(par));				// initialize particle analysis report
	
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
	//td1.luminance.minValue = 85;
	td1.luminance.minValue = 60;
	//td1.luminance.maxValue = 255;
	td1.luminance.maxValue = 180;
	
	// GREEN
	sprintf(td2.name, "GREEN");
	td2.hue.minValue = 55;
	td2.hue.maxValue = 125;
	td2.saturation.minValue = 58;
	td2.saturation.maxValue = 255;
	//td2.luminance.minValue = 92;
	td2.luminance.minValue = 70;
	//td2.luminance.maxValue = 255;
	td2.luminance.maxValue = 180;
	
	sprintf(custom1.name, "PINK");
	custom1.R.minValue = 240;
	custom1.R.maxValue = 255;
	custom1.G.minValue = 79;
	custom1.G.maxValue = 153;
	custom1.B.minValue = 128;
	custom1.B.maxValue = 179;
	
	sprintf(custom2.name, "GREEN");
	custom2.R.minValue = 116;
	custom2.R.maxValue = 160;
	custom2.G.minValue = 217;
	custom2.G.maxValue = 250;
	custom2.B.minValue = 117;
	custom2.B.maxValue = 150;
	
	masterControl = NULL;
	
	mode = MODE_AUTO;
	
	max_encoder_voltage = 3.5;
	min_encoder_voltage = 1.7;
#if USE_PID
	pid = new PIDController(1.0,	// Use proportional
							0.0,	// Don't use derivative
							0.0,		// Don't use integral
							Position_Encoder,
							Turret_Pos_Motor); 

	// Set the input range to reflect knowledge
	// of our encoder 
	pid->SetInputRange(0, 5);
	pid->SetOutputRange(-1,1);
#endif	
	//pid->SetSource(Position_Encoder);
	//pid->SetOutput(Turret_Pos_Motor, -1.0, 1.0);
	
	panIncrement = 0;
	staleImage = false;
	
	savedImageTimestamp = 0.0;
	servoDeadband = 0.01;					// move if > this amount 
	framesPerSecond = 15;					// number of camera frames to get per second
	sinStart = 0.0;							// control where to start the sine wave for pan
	
	//cameraRotation = ROT_0;					// input parameter for camera orientation
			
}

Turret::~Turret()
{
	delete Turret_Pos_Motor;
	delete Clockwise_Limit;
	delete CounterClockwise_Limit;
	delete Position_Encoder;
}

void Turret::RegisterMasterControl(int *ptr)
{
	this->masterControl = ptr;
}
void Turret::TurretControl(void)
{
	Auto();
}
void Turret::TurretControl(Joystick * turretStick)
{
	switch (mode)
	{
	case MODE_MANUAL:	// Manual
		Manual(turretStick);
		//ManualPositionMode(turretStick);
		break;
	case MODE_SEMI_AUTO: // Semi-Automatic (BANG! BANG!)
		ManualPositionMode(turretStick);
		//ManualPositionMode(turretStick);
		break;
	case MODE_AUTO:	// Fully automatic AEGIS-style, full robot control
		//Auto();
		Auto();
		break;
	default:
		Manual(turretStick);
	}
	//Manual(turretStick);
}


void Turret::Manual(Joystick *turretStick)
{
	EndServoish();
	// Read Joystick X Axis
	float x_axis = turretStick->GetX();
	
	// Scale value down 2x?
	x_axis *= .60;
	// Limit x_axis
	
	// Set motor to scaled value
	if (x_axis < 0) {
		Clockwise_Limit->LimitNegative(x_axis);
	}
	else {
		CounterClockwise_Limit->LimitPositive(x_axis);
	}
	
	
	Turret_Pos_Motor->Set(x_axis);
	
	DriverStationLCD *dsLCD = DriverStationLCD::GetInstance();
	dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "TSMAN:%.3f",x_axis);
	//dsLCD->Printf(DriverStationLCD::kUser_Line6, 2, "Encoder :%7.1f V", this->EncoderVoltage());
	
	UpdateState();
	return; // Guess what? return.
}

float Turret::CurrentPosition(void)
{
	// Range: -1 to 1
	return Position_Encoder->GetVoltage();
}

float Turret::EncoderVoltage(void)
{
	return Position_Encoder->GetVoltage(); 
}

void Turret::ManualPositionMode(Joystick *turretStick)
{
	InitServoish();
	SetTurretPosition((turretStick->GetX()+1.0)/2);
	
	//UpdateState();
}
void Turret::Auto(void)
{
	InitServoish();
	Target();
	Track();
	UpdateState();
}

void Turret::Target(void)
{
	// if ( FindTwoColors(td1, td2, BELOW, &par) ){
	if ( FindTwoColorsRGB(custom1, custom2, BELOW, &par) ){
			// We found a target
			
			//theRobot->GetTheDashboard().Printf("Target found-- (%d %d) h: %d w: %d", par.boundingRect.top, par.boundingRect.left,
	//				par.boundingRect.height, par.boundingRect.left);
			
		this->tracking = true;
	} else {
			// We don't have a target
	//		theRobot->GetTheDashboard().Printf("No target in sight.");
		this->tracking = false;
	}
}

void Turret::Track(void)
{
	if ( this->TargetInSight() ){
					//PrintReport(&par);
					//foundColor = true;
					// reset pan		
					panIncrement = 0;  		
					if (par.imageTimestamp == savedImageTimestamp) {
						// This image has been processed already, 
						// so don't do anything for this loop 
						staleImage = true;
						//DPRINTF(LOG_DEBUG, "STALE IMAGE");
						
					} else {
						// The target was recognized
						// save the timestamp
						staleImage = false;
						savedImageTimestamp = par.imageTimestamp;	
						//DPRINTF(LOG_DEBUG,"image timetamp: %lf", savedImageTimestamp);

						// Here is where your game-specific code goes
						// when you recognize the target
						
						// get center of target 
						
						// TODO: use par.average_mass_x_normalized and
						// par.average_mass_y_normalized after updated WPILib is distributed
						horizontalDestination = par.center_mass_x_normalized;	
						//verticalDestination = par->center_mass_y_normalized;						
					}
				} else {  // need to pan 
					//foundColor = false;
				} 
									
				if(this->TargetInSight() && !staleImage) {	
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
					AdjustServoPositions( incrementH, incrementV );  
					
					/* ShowActivity ("** %s & %s found: Servo: x: %f  y: %f ** ", 
							td1.name, td2.name, horizontalDestination, verticalDestination); */	
					
				} else { //if (!staleImage) {  // new image, but didn't find two colors
					
					// adjust sine wave for panning based on last movement direction
					if(horizontalDestination > 0.0)	{ sinStart = PI/2.0; }
					else { sinStart = -PI/2.0; }

					/* pan to find color after a short wait to settle servos
					 * panning must start directly after panInit or timing will be off */				
					if (panIncrement == 3) {
						panInit(8.0);		// number of seconds for a pan
					}
					else if (panIncrement > 3) {					
						//panForTarget(horizontalServo, sinStart);	
						
						/* Vertical action: In case the vertical servo is pointed off center,
						 * center the vertical after several loops searching */
						//if (panIncrement == 20) { verticalServo->Set( 0.5 );	}
					}
					panIncrement++;		

					// ShowActivity ("** %s and %s not found                                    ", td1.name, td2.name);
				}  // end if found color

				// sleep to keep loop at constant rate
				// this helps keep pan consistant
				// elapsed time can vary significantly due to debug printout
				//currentTime = GetTime();			
				//lastTime = currentTime;					
				//if ( loopTime > ElapsedTime(lastTime) ) {
				//	Wait( loopTime - ElapsedTime(lastTime) );	// seconds
				//}			
}
double Turret::GetTarget_X()
{	
	return par.center_mass_x_normalized;
}

void Turret::SetTurretPosition(float position)
{
#if USE_PID
	pid->SetSetpoint(ServoToEncoderUnits(position));
#else
	float error = ServoToEncoderUnits(position) - EncoderVoltage();
	
	float output = 1.0 / Turret::kEncoderRange * error;
	
	float x_axis = output * 1;
	
	if (x_axis < 0) {
		Clockwise_Limit->LimitNegative(x_axis);
	}
	else {
		CounterClockwise_Limit->LimitPositive(x_axis);
	}
	
	DriverStationLCD * dsLCD = DriverStationLCD::GetInstance();
	dsLCD->Printf(DriverStationLCD::kUser_Line2, 1, "TSPID:%.3f",x_axis);
	dsLCD->UpdateLCD();
	
#if ENABLE_TURRET
	Turret_Pos_Motor->Set(x_axis);
#else
	Turret_Pos_Motor->Set(0.0);
#endif
#endif 
}

void Turret::AdjustServoPositions(float normDeltaHorizontal, float normDeltaVertical)	{
						
		/* adjust for the fact that servo overshoots based on image input */
		normDeltaHorizontal /= 8.0;
		normDeltaVertical /= 4.0;
		
		/* compute horizontal goal */
		//float currentH = horizontalServo->Get();
		//float currentH = 
		//float normCurrentH = RangeToNormalized(currentH, 1);
		float normCurrentH = TurretPosition();
		float currentH = (normCurrentH*2) - 1;
		float normDestH = normCurrentH + normDeltaHorizontal;	
		/* narrow range keep servo from going too far */
		if (normDestH > 1.0) normDestH = 1.0;
		if (normDestH < -1.0) normDestH = -1.0;			
		/* convert input to servo range */
		float servoH = NormalizeToRange(normDestH);

		/* compute vertical goal */
#if 0
		float currentV = verticalServo->Get();
		float normCurrentV = RangeToNormalized(currentV, 1);
		float normDestV = normCurrentV + normDeltaVertical;	
		if (normDestV > 1.0) normDestV = 1.0;
		if (normDestV < -1.0) normDestV = -1.0;
		/* convert input to servo range */
		float servoV = NormalizeToRange(normDestV, 0.2, 0.8);
#endif
		/* make sure the movement isn't too small */
		if ( fabs(currentH-servoH) > servoDeadband ) {
			//horizontalServo->Set( servoH );
			SetTurretPosition(servoH);
			/* save new normalized horizontal position */
			horizontalPosition = RangeToNormalized(servoH, 1);
		}
#if 0
		if ( fabs(currentV-servoV) > servoDeadband ) {
			verticalServo->Set( servoV );
			verticalPosition = RangeToNormalized(servoV, 1);
		}
#endif
	}
float Turret::TurretPosition(void)
{
	return EncoderUnitsToServo(EncoderVoltage());
	//return (1.0 - (max_encoder_voltage - GetEncoderVoltage()) / (max_encoder_voltage - min_encoder_voltage));
	//return GetEncoderVoltage();
}

void Turret::UpdateState(void)
{
	// Update maximum and minimum observed encoder voltages for range
	if (EncoderVoltage() < min_encoder_voltage) min_encoder_voltage = EncoderVoltage();
	else if (EncoderVoltage() > max_encoder_voltage) max_encoder_voltage = EncoderVoltage();
	
	
	// Set motor to scaled value
	float x_axis = Turret_Pos_Motor->Get();
		if (x_axis < 0) {
			Clockwise_Limit->LimitNegative(x_axis);
		}
		else {
			CounterClockwise_Limit->LimitPositive(x_axis);
		}
		
	Turret_Pos_Motor->Set(x_axis);
	
}

void Turret::InitServoish(void)
{
#if USE_PID
	pid->Enable();
#endif
}

void Turret::EndServoish(void)
{
#if USE_PID
	pid->Disable();
#endif
}

inline float Turret::ServoToEncoderUnits(float servo)
{
	//return Turret::kCWVoltage + 
	return Turret::kCCWVoltage + (servo * Turret::kEncoderRange);
	//return min_encoder_voltage + (servo * (max_encoder_voltage - min_encoder_voltage));
}

inline float Turret::EncoderUnitsToServo(float volts)
{
	//return (volts - min_encoder_voltage) / (max_encoder_voltage - min_encoder_voltage);
	return (volts - Turret::kCCWVoltage) / Turret::kEncoderRange;
}
