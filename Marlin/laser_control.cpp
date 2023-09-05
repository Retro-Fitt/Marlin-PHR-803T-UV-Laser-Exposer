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
//#include "MarlinConfig.h"
#include "laser_control.h"

bool static laser_on = false;
/**
 ** laser_init() - Inits the laser subsystem
 **/
void laser_init()
{
  laser_on = false;
  SET_OUTPUT(LASER_CURRENT_PIN);
  SET_OUTPUT(LASER_ENABLE_PIN);
  TCNT0  = 0;
  OCR0A = 0;
  OCR0B = 128;
  TCCR0A = 0;
  TCCR0A |= ((1 << WGM01) | (1 << WGM00)); // Fast PWM mode on pin 5 and 6
  TCCR0B = 0;
  TCCR0B |=  (1 << CS00); // No prescale 65000Khz.
}


/**
 ** set_laser_power() - Sets the intensity of laser
 **/
void set_laser_power(uint8_t level){
  SET_OUTPUT(LASER_CURRENT_PIN);
  TCCR0A |= (1 << COM0A1);
  OCR0A = level;
}
/**
 ** get_laser_power() - Get the intensity of laser
 **/
uint8_t get_laser_power(){
  return OCR0A;
}

/**
** turn_laser() - Fires the laser on/off
**/
void turn_laser(uint8_t mode){
	if (mode == ON){
		WRITE(LASER_ENABLE_PIN,HIGH);
		laser_on = true;
	}
	else {
		WRITE(LASER_ENABLE_PIN,LOW);
		laser_on = false;
	}
}


/**
 ** is_laser_on() - Returns true is laser is on
 **/
bool is_laser_on(){
  return laser_on;
}

/**
 ** move_focus_servo() - Moves the lens of the pickup
 **/
void move_focus_servo(uint8_t pos){
  //delay_us(3);
  delayMicroseconds(3); //ADDED
  SET_OUTPUT(FSERVO_STEP_PIN);
  if (pos == 0){
    WRITE(FSERVO_STEP_PIN,LOW);
  } 
  else if (pos == 255){
    WRITE(FSERVO_STEP_PIN,HIGH);
  }
  else {
    TCCR0A |= (1 << COM0B1);
    OCR0B = pos;
  }
}



