/*
  focus.h - laser focus module header
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

#ifndef	FOCUS_H
#define	FOCUS_H

#define LASER_FOCUS_POWER   128 //IMPLEMENTED ON FOCUS.CPP
#define FOCUS_ERROR			0xff
#define FOCUS_OK			0

// FE values

#define MIN_FE_VALUE	0		//Min focus error signal val 
#define MAX_FE_VALUE	1023	//Max focus error signal val
#define REF_FE_VALUE	505		//Typical Vref digital value

// Focus Servo

#define FSERVO_MIN         0	// Focus servo min position (inside pickup)
#define FSERVO_MAX         230	// Focus servo max position (outside)
#define FSERVO_CENTER      128  // Focus servo center
#define MAX_FOCUS_SWEEP    5    // Max focus attempts
#define NUM_FOCUS_SAMPLES  15   // Num focus samples
#define FOCUS_TIMEOUT      30000  // Focus timeout in ADC ticks at 2000Khz.

#define FE_MIN_AMP_THRESHOLD  128  // No object limit //MODIFIED FOR TESTING WAS 128
#define FE_NOISE_DIVIDER	   3  // Used to find the noise threshold
#define FE_AMPLITUD_DIVIDER   48  // Amplitud/(this) is the focus threshold 

#define FE_WAIT_ST         0	// Waiting
#define FE_AMPLITUD_ST     1	// Scurve amplitud measure
#define FE_FOCUS_ST        2	// Begin focus state
#define FE_SCURVE_ST       3	// Scurve detected
#define FE_FOCUSED_ST      4	// Focused
#define FE_ERROR_ST        5    // Timeout error

uint8_t auto_focus();
uint8_t get_focus_position();
void start_fe_sampler();
void stop_fe_sampler();
void set_state(uint8_t state);
uint8_t calc_mode(uint8_t *value, uint8_t num);

#endif
