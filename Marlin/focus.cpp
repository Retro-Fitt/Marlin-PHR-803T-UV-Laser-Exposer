/*
  focus.c - Blue-ray Pickup laser focus module
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
#include "focus.h"


uint16_t fe_max_val = MIN_FE_VALUE;	        // Is OK
uint16_t fe_min_val = MAX_FE_VALUE;	        // Is OK

volatile uint8_t fe_servo_pos = 0;
volatile uint8_t fe_state = FE_WAIT_ST;
volatile uint16_t fe_amplitud = 0;
volatile uint8_t fe_focus_pos = 0;

/**
 ** auto_focus() - Focus the laser
 **/
uint8_t auto_focus(){
  uint8_t fpos;
  uint8_t val;
  uint8_t prev_laser_power;
  uint8_t focus_sample[NUM_FOCUS_SAMPLES];
  //
  // Save laser power
  // Set laser to focus level
  //
  prev_laser_power = get_laser_power();
  set_laser_power(LASER_FOCUS_POWER);
  //
  // Get focus position
  // Try some times and get average focus
  //
  uint8_t i = 0;

  do {
	  if ((fpos = get_focus_position()) != FOCUS_ERROR){
		  if (fpos > FE_MIN_AMP_THRESHOLD){
			SERIAL_ECHOLN((int)fpos);
			focus_sample[i++] = (uint8_t)fpos;
		  }
		  //delay_ms(100);
      delayMicroseconds(100000); //ADDED
	  } else {
		  set_laser_power(prev_laser_power);
		  turn_laser(OFF);
		  return FOCUS_ERROR;
	  }
  } while  (i < NUM_FOCUS_SAMPLES);

  //
  // We have some Focus positions
  // Calc mode, focus the laser & restore
  // previous laser power
  val = calc_mode(focus_sample,NUM_FOCUS_SAMPLES);
  move_focus_servo(val); 
  //
  set_laser_power(prev_laser_power);
  turn_laser(OFF);
  return val;
}

/**
 ** get_focus_position() - Determines lens servo best focus position
 **/
uint8_t get_focus_position(){
  //
  // Try to get amplitud and focus some times
  //
  for (uint8_t j = 0; j < MAX_FOCUS_SWEEP; j++){
    // Turn on laser
    turn_laser(ON);
    //
    // Move servo to max position (close to the PCB)
    //
    move_focus_servo(FSERVO_MAX);
	//delay_ms(5);
  delayMicroseconds(5000); //ADDED
    // Start to sampling and 
    // set state to read S-curve amplitud
    start_fe_sampler();
    set_state(FE_AMPLITUD_ST);
    // Init variables
    fe_max_val = MIN_FE_VALUE;
    fe_min_val = MAX_FE_VALUE;
    fe_amplitud = 0;
    //
    // move servo from up to Down
    //
    for (fe_servo_pos = FSERVO_MAX; fe_servo_pos > FSERVO_MIN; fe_servo_pos--){
      move_focus_servo(fe_servo_pos);  
    }
    //
    // Set state to wait, while calc. amplitud
    //
    set_state(FE_WAIT_ST);
    //
    // Get Amplitud of S-Curve
    // 
    fe_amplitud = (fe_max_val-fe_min_val);
    //
    // If Amplitud has a miminum threshold, try to focus
    //
    if (fe_amplitud > FE_MIN_AMP_THRESHOLD){
      // Move servo down to up
      // and monitor FE value. When fe_state == FE_FOCUSED_ST
      // the laser is focused.
      set_state(FE_FOCUS_ST);

	  for (;;){
        if (fe_servo_pos++ == FSERVO_MAX)
			break;
        move_focus_servo(fe_servo_pos);
        cli();
		if (fe_state == FE_ERROR_ST){
			sei();
			break;
		}
        if (fe_state == FE_FOCUSED_ST){ 
          // Focused!!
          // Stop sampling and return FocusPosition      
          stop_fe_sampler();
          turn_laser(OFF);
          return fe_focus_pos; 
        }
        sei();
      }
    }
    stop_fe_sampler();
    turn_laser(OFF);
  }
  //
  // Max attempts, stop sampling and return error
  //
  return FOCUS_ERROR;
}

/**
 ** start_fe_sampler() - Starts the sampling of data
 **/
void start_fe_sampler(){
  cli();

  //clear ADCSRA and ADCSRB registers
  ADCSRA = 0;
  ADCSRB = 0;
  // Init ADC
  ADMUX = ((1 << REFS0) | ((FE_SIGNAL_PIN-PIN_A0) & 0x07));
  ADCSRA |= (1 << ADPS1) | (1 << ADPS0); //set ADC clock with 32 prescaler- 16mHz/4=2000kHz
  ADCSRA |= (1 << ADATE); //enabble auto trigger
  ADCSRA |= (1 << ADIE); //enable interrupts when measurement complete
  ADCSRA |= (1 << ADEN); //enable ADC
  ADCSRA |= (1 << ADSC); //start ADC measurements
  // Set analog input
  DIDR0 = 0;
  DIDR0 |= (1<<(FE_SIGNAL_PIN-PIN_A0)); 
  fe_state = FE_WAIT_ST;
  sei();
}
/**
 ** stop_fe_sampler() - Stop the sampling of data
 **/
void stop_fe_sampler(){
  cli();
  ADCSRA = 0;
  ADCSRB = 0;
  fe_state = FE_WAIT_ST;
  sei();
}
/**
 ** set_state() - Sets the state of sampling
 **/
void set_state(uint8_t state){
  cli();
  fe_state = state;
  sei();
}
/**
 ** Interrupt vector
 **/
ISR(ADC_vect) {//when new ADC value ready
	static uint16_t FEVal;
	static uint16_t FEtimeout;
	ADCSRB = 0;
	ADMUX = ((1 << REFS0) | ((FE_SIGNAL_PIN-PIN_A0) & 0x07));

	// State machine
	switch (fe_state){
	case FE_WAIT_ST:
		FEtimeout=0;
		break;
	case FE_AMPLITUD_ST:
		ADCSRA |= 1<<ADSC; // Start conversion
		FEVal = ADC;
		if ( FEVal > fe_max_val)
			fe_max_val = FEVal;
		if (FEVal < fe_min_val)
			fe_min_val = FEVal;
		break;
	case FE_FOCUS_ST:
		ADCSRA |= 1<<ADSC; // Start conversion
		FEVal = ADC;
		if (abs(REF_FE_VALUE - FEVal) > fe_amplitud/FE_NOISE_DIVIDER){
			FEtimeout=0;
			fe_state = FE_SCURVE_ST;
		} 
		else if (FEtimeout++ > FOCUS_TIMEOUT){
			fe_state = FE_ERROR_ST;
		}
		break;
	case FE_SCURVE_ST:
		ADCSRA |= 1<<ADSC; // Start conversion
		FEVal = ADC;
		if (abs(REF_FE_VALUE - FEVal) <= fe_amplitud/FE_AMPLITUD_DIVIDER){
			fe_focus_pos = fe_servo_pos;
			fe_state = FE_FOCUSED_ST;
		} 
		else if (FEtimeout++ > FOCUS_TIMEOUT){
			fe_state = FE_ERROR_ST;
		}
		break;
	case FE_FOCUSED_ST:
		break;
	case FE_ERROR_ST:
		break;
	}
}
/**
 ** calc_mode() - Calculates the mode of an array of unsigned int numbers
 **/
uint8_t calc_mode(uint8_t *value, uint8_t num)
{
  uint8_t count, old_count;
  uint8_t mode, old_mode;

  old_count = mode =  old_mode = 0;
  uint8_t i;
  for(i=0; i<num; i++) {
    mode = value[i];
    count = 1;
    uint8_t j;
    for(j = i+1; j < num; j++) 
      if(mode==value[j]) count++;
    if(count > old_count) {
      old_mode = mode;
      old_count = count;
    }
  }
  return old_mode;
}

