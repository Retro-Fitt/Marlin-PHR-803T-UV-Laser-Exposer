/*
  laser_control.c - laser control module
 Part of DiyouPCBFirm
 
 Copyright (c) 2013 V.Ayllon.  All rights reserved.
 
 DiyouPCBFirm is distributed in the hope that it will be useful, but
 WITHOUT ANY WARRANTY.  No author or distributor accepts responsibility
 to anyone for the consequences of using it or for whether it serves any
 particular purpose or works at all, unless he says so in writing.  Refer
 to the GNU General Public License, Version 2 or later, for full details.
 
 Everyone is granted permission to copy, modify and redistribute
 DiyouPCBFirm, but only under the conditions described in the GNU General Public
 License.  A copy of this license is supposed to have been
 given to you along with DiyouPCB so you can know your rights and
 responsibilities.  It should be in a file named COPYING.  Among other
 things, the copyright notice and this notice must be preserved on all
 copies.
 
 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 */
#include "Marlin.h"
#include "laser_control.h"

//bool static laser_on = false;
/**
 ** laser_init() - Inits the laser subsystem //CHECKED OK
 **/
void laser_init()
{
  laser_on = false;
    SET_OUTPUT(LASER_ENABLE_PIN);
  	// Set laser focus step (5) pin, OC3A Timer 3, fast pwm
	SET_OUTPUT(FSERVO_STEP_PIN);
	OCR3AH = 0;
	OCR3AL = 128;
	TCCR3A = 0;
	TCCR3B = 0;
	TCCR3C = 0;
	TCCR3A |= (1 << WGM30); // Fast PWM mode on pin 5
	TCCR3B |=  (1 << CS30); // No prescale 65000Khz.

	// Set laser current (6) pin OC4A Timer 4, fast pwm //CHECKED OK
	SET_OUTPUT(LASER_CURRENT_PIN);
	OCR4AH = 0;
	OCR4AL = 0;
	TCCR4A = 0;
	TCCR4B = 0;
	TCCR4C = 0;
	TCCR4A |= (1 << WGM40); // Fast PWM mode on pin 5
	TCCR4B |= (1 << CS40); // No prescale 65000Khz.
}


/**
** laser_end() - End the laser subsystem
**/
void laser_end()
{
	turn_laser(OFF); //TO BE CHECKED
	set_laser_power_off();
	OCR3AH = 0;
	OCR3AL = 0;
	OCR4AH = 0;
	OCR4AL = 0;
	TCCR3A = 0;
	TCCR3B = 0;
	TCCR3C = 0;
	TCCR4A = 0;
	TCCR4B = 0;
	TCCR4C = 0;
}



/**
** move_focus_servo() - Moves the lens of the pickup
**/
void move_focus_servo(uint8_t pos){
	delay_us(3); //WAS 3
	//delayMicroseconds(3); //ADDED
	SET_OUTPUT(FSERVO_STEP_PIN);
	set_focus_pos(pos);
}
