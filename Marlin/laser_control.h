/*
  laser_control.h - laser control code
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

#ifndef LASER_CONTROL_H
#define LASER_CONTROL_H

bool static laser_on = false;

void laser_init();
//void turn_laser(uint8_t mode);
//void set_laser_power(uint8_t level);
//uint8_t get_laser_power();
//void move_focus_servo(uint8_t pos);
//bool is_laser_on();

void laser_end();
void move_focus_servo(uint8_t pos);


/**
** set_laser_on() - Fires the laser modifing power level 0 - 128
** It's assumed that laser was previously turned on with turn_laser_on()
**/
FORCE_INLINE void set_laser_power(uint8_t level){	
	TCCR4A |= (1 << COM4A1);
	OCR4AH = 0;
	OCR4AL = level;
}

/**
** set_laser_power_off() - Stop the laser but modifing power level
** It's assumed that laser was previously turned on with turn_laser_on().
**/
FORCE_INLINE void set_laser_power_off(){

	TCCR4A |= (1 << COM4A1);
	OCR4AH = 0;
	OCR4AL = 0;
}


/**
** get_laser_power() - Get the intensity of laser
**/
FORCE_INLINE uint8_t get_laser_power(){
	return OCR4AL;
}

/**
** turn_laser() - Fires the laser on/off
**/
FORCE_INLINE void turn_laser(uint8_t mode){
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
** set_focus_pos() - Moves the focus lens (Servo) to the focus position
**/
FORCE_INLINE void set_focus_pos(uint8_t pos){ //ADDED
	TCCR3A |= (1 << COM3A1); //ADDED
	OCR3AH = 0; //ADDED
	OCR3AL = pos; //ADDED
}

#endif
