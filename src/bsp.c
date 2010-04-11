/*********************************************
 *
 * bsp.c
 * 
 * Initializes I/O pins and clock/timer settings
 * 
 *
 * Copyright 2009 Spencer Russell
 * Released under the GPLv3
 *
 * This file is part of The ProxLamp Firmware.
 *
 * The ProxLamp Firmware is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * The ProxLamp Firmware is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * The ProxLamp Firmware.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 ********************************************/



#include "bsp.h"
#include <avr/interrupt.h>
#include <avr/io.h>
void bsp_setup() {
	clock_setup();
	triac_timer_setup();
	sensor_timer_setup();
	pin_io_setup();
	/* enable interupts */
	sei();
}

void clock_setup(void) {
	/* set the main clock prescaler to 1 */
	CLKPR = 1 << CLKPCE;
	CLKPR = 0;
}

void triac_timer_setup() {
	/* run counter in normal mode */
	TRIAC_CFG_A = 0x00;
	/* set up 16-bit counter with prescalar of 8 */
	TRIAC_CFG_B = (1 << CS11);
}

void sensor_timer_setup() {
	/* run counter in clear-on-compare mode */
	SENSOR_CFG_A = (1 << WGM01);
	/* set up 8-bit counter with no prescalar */
	SENSOR_CFG_B = (1 << CS00);
	SENSOR_COMP = SENSOR_TCNT_MAX;
	/* disable timer interrupts for this counter */
	SENSOR_TIMSK &= ~SENSOR_INT_EN;
	/* set compare value to zero (effectively trigger on overflow) */
	SENSOR_COMP = 0;
}

void pin_io_setup(void) {
	/* start triac pin in low state */
	TRIAC_PORT &= ~TRIAC_PIN; 
	/* set the triac pin to output */
	TRIAC_DD |= TRIAC_PIN; 

	/* start in low state */
	TEST_PORT &= ~TEST_PIN; 
	/* set the test pin to output */
	TEST_DD |= TEST_PIN; 

	/* disable pullup resistor on zero-cross detector */
	ZEROCROSS_PORT &= ~ZEROCROSS_PIN;
	/* set the zero cross detector pin to input */
	ZEROCROSS_DD &= ~ZEROCROSS_PIN; 

	/* disable pullup resistor on receiver pin */
	RECEIVE_PORT &= ~RECEIVE_PIN;
	/* set the receiver pin to input */
	RECEIVE_DD &= ~RECEIVE_PIN; 

	/* set all trigger pins low */
	SENSOR_TRIGGER_PORT &= ~SENSOR_TRIGGER_MASK;
	/* set all trigger pins as output */
	SENSOR_TRIGGER_DD |= SENSOR_TRIGGER_MASK;

	/* start sensor select pins 000 */
	SENSOR_SEL_PORT &= ~SENSOR_SEL_MASK;
	/* set the sensor selector pins to output */
	SENSOR_SEL_DD |= SENSOR_SEL_MASK;
	
	/* set Zerocross interrupt to trigger on both edges */
	ZEROCROSS_EICRA |= (1 << ISC00);

	/* enable interrupt from INT0 */
	ZEROCROSS_INT_ENABLE();

	/* set receive interrupt to trigger on both edget */
	RECEIVE_EICRA |= (1 << ISC10);

	/* disable receive interrupt until we're ready to listen */
	RECEIVE_INT_DISABLE();
}

