#include "Turret.h"

Turret::Turret() {

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
	
	/*
	custom1.hue.minValue = 300;
	custom1.hue.maxValue = 350;
	custom1.saturation.minValue = 22;
	custom1.saturation.maxValue = 43;
	custom1.luminance.minValue = 90;
	custom1.luminance.maxValue = 110;
	
	custom2.hue.minValue = 100;
	custom2.hue.maxValue = 120;
	custom2.saturation.minValue = 35;
	custom2.saturation.maxValue = 50;
	custom2.luminance.minValue = 90;
	custom2.luminance.maxValue = 100;
	*/
}

Turret::~Turret()
{
	
}

void Turret::TurretControl(void)
{
	if ( FindTwoColors(td1, td2, BELOW, &par) ){
	//if ( FindTwoColors(custom1, custom2, BELOW, &par) ){
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
