#include "Turret.h"
#include "DriverStationLCD.h"

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
	
	max_encoder_voltage = 3.5;
	min_encoder_voltage = 1.7;
	
	pid = new PIDController(1.0,	// Use proportional
							0.0,	// Don't use derivative
							0.0,		// Don't use integral
							Position_Encoder,
							Turret_Pos_Motor); 
	
	//pid->SetSource(Position_Encoder);
	//pid->SetOutput(Turret_Pos_Motor, -1.0, 1.0);
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
	switch (*masterControl)
	{
	case 0:	// Manual
		//Manual(turretStick);
		ManualPositionMode(turretStick);
		break;
	case 1: // Semi-Automatic (BANG! BANG!)
		//Manual(turretStick);
		ManualPositionMode(turretStick);
		break;
	case 2:	// Fully automatic AEGIS-style, full robot control
		Auto();
		break;
	default:
		Manual(turretStick);
	}
	Manual(turretStick);
}


void Turret::Manual(Joystick *turretStick)
{
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
	
	dsLCD->Printf(DriverStationLCD::kUser_Line6, 2, "Encoder :%7.1f V", this->EncoderVoltage());
	
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
	SetTurretPosition((turretStick->GetX()+1.0)/2);
	
	UpdateState();
}
void Turret::Auto(void)
{
	Target();
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
double Turret::GetTarget_X()
{	
	return par.center_mass_x_normalized;
}

void Turret::SetTurretPosition(float position)
{
	pid->SetSetpoint(ServoToEncoderUnits(position));
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
	pid->Enable();
}

void Turret::EndServoish(void)
{
	pid->Disable();
}

inline float Turret::ServoToEncoderUnits(float servo)
{
	return min_encoder_voltage + (servo * (max_encoder_voltage - min_encoder_voltage));
}

inline float Turret::EncoderUnitsToServo(float volts)
{
	return (volts - min_encoder_voltage) / (max_encoder_voltage - min_encoder_voltage);
}
