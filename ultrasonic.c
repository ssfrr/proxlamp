/*********************************************
 *
 * ultrasonic.c
 * 
 * Sends ultrasonic pulses and handles the echos
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
#include "ultrasonic.h"
#include "sched.h"

volatile unsigned int periods;

int get_waiting_tics() {
	return (periods * SENSOR_TCNT_MAX + SENSOR_TCNT) << SENSOR_TIME_DIV;
}

void select_sensor(unsigned char sensor) {
	SENSORbits.SELECT = sensor;
}

void send_pulse(unsigned int pulses) {
	state = PULSING;
	/* set pin as output */
	SENSOR_DD = (1 << sensor);
	/* set the compare value to the pulse length */
	SENSOR_COMP = SENSOR_PULSE;
	/* we're actually counting half-cycles */
	periods = pulses << 1;
	SENSOR_TCNT = 0;
	/* clear any existing sensor timer flags */
	SENSOR_TIFR |= SENSOR_INT_FLAG;
	/* enable sensor timer interrupt */
	SENSOR_TIMSK |= SENSOR_INT_EN;
}

ISR(INT_SENSOR_TIMER) {
	if(state == PULSING) {
		if(periods--) {
			/* while we still have pulses to send */
			/* toggle the current sensor pin */
			SENSOR_PORT ^= (1 << sensor);
		}
		else {
			/* now we wait for the echo */
			state = WAITING;
			/* we're going to count how many overflows occur */
			SENSOR_COMP = SENSOR_TCNT_MAX;
		}
	}
	else if(state == WAITING) {
		periods++;
	}
}
