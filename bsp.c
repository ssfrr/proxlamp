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



#include "proxlamp_r01_bsp.h"
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
	TRIAC_CONF_A = 0x00;
	/* set up 16-bit counter with prescalar of 8 */
	TRIAC_CONF_B = (1 << CS11);
}

void sensor_timer_setup() {
	/* run counter in clear-on-compare mode */
	SENSOR_CONF_A = (1 << WGM01);
	/* set up 8-bit counter with no prescalar */
	SENSOR_CONF_B = (1 << CS00);
	SENSOR_COMP = SENSOR_TCNT_MAX;
	/* disable timer interrupts for this counter */
	SENSOR_TIMSK &= ~SENSOR_INT_EN;
}

void pin_io_setup(void) {
	/* set the triac pin to output */
	TRIAC_DD |= TRIAC_PIN; 
	/* start in low state */
	TRIAC_PORT &= ~TRIAC_PIN; 

	/* set the test pin to output */
	TEST_DD |= TEST_PIN; 
	/* start in low state */
	TEST_PORT &= ~TEST_PIN; 

	/* set the pulse pin to outpue */
	SENSOR_PULSE_DD |= SENSOR_PULSE_PIN;
	/* start the pulse pin at 0 */
	SENSOR_PULSE_PORT |= SENSOR_PULSE_PIN;

	/* set the sensor direction select pin to output */
	SENSOR_DIR_DD |= SENSOR_DIR_PIN;
	/* set the sensor directino select pin to send */
	SET_SENSOR_SEND();

	/* set the zero cross detector pin to input */
	ZEROCROSS_DD &= ~ZEROCROSS_PIN; 
	/* disable pullup resistor on zero-cross detector */
	ZEROCROSS_PORT &= ~ZEROCROSS_PIN;

	/* set the receiver pin to input */
	RECEIVE_DD &= ~RECEIVE_PIN; 
	/* disable pullup resistor on receiver pin */
	RECEIVE_PORT &= ~RECEIVE_PIN;

	/* set the sensor selector pins to output */
	SENSOR_SEL_DD |= SENSOR_SEL_MASK;
	/* start sensor select pins 000 */
	SENSOR_SEL_PORT &= ~SENSOR_SEL_MASK;
	
	/* set Zerocross interrupt to trigger on both edges */
	ZEROCROSS_EICRA |= (1 << ISC00);

	/* enable interrupt from INT0 */
	ZEROCROSS_INT_ENABLE();

	/* set receive interrupt to trigger on both edget */
	RECEIVE_EICRA |= (1 << ISC10);

	/* disable receive interrupt until we're ready to listen */
	RECEIVE_INT_DISABLE();
}

