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

static volatile unsigned int periods;
volatile unsigned int echo_tics = 0;

/* calculate the counter value for the transmit frequency */
#define SENSOR_PULSE ((F_CPU / FTRANS / 2) >> SENSOR_TIME_DIV)

#define RINGDOWN_PERIODS (RINGDOWN_US * (F_CPU / 1000000) \
		/ (SENSOR_TCNT_MAX >> SENSOR_TIME_DIV))
#define TIMEOUT_PERIODS (TIMEOUT_US* (F_CPU / 1000000) \
		/ (SENSOR_TCNT_MAX >> SENSOR_TIME_DIV))


/* set up the hardware for the proper sensor */
void select_sensor(unsigned char sensor) {
	SENSOR_SELECT(sensor);
}

/* send a number of ultrasonic pulses at FTRANS Hz */
void send_pulses(unsigned int pulses) {
	state = PULSING;
	echo_tics = 0;
	/* set the timer compare value to the pulse length */
	SENSOR_COMP = SENSOR_PULSE;
	/* we're actually counting half-cycles */
	periods = pulses << 1;
	SET_SENSOR_SEND();
	SENSOR_TCNT = 0;
	/* clear any existing sensor timer flags */
	SENSOR_TIMER_INT_CLEARFLAG();
	/* enable sensor timer interrupt */
	SENSOR_TIMER_INT_ENABLE();
}

/* 
 * this interrupt is triggered every time the sensor counter hits
 * SENSOR_COMP. The counter is automatically set to 0.
 */
ISR(INT_SENSOR_TIMER) {
	switch(state) {

		case PULSING:
		if(periods--) {
			/* while we still have pulses to send */
			TOGGLE_PULSE_PIN();
		}
		else {
			/* now we wait for the resonance to die down*/
			state = IGNORING;
			/* we're going to count how many overflows occur */
			SENSOR_COMP = SENSOR_TCNT_MAX;
		}
		break;

		case IGNORING:
		if(periods > RINGDOWN_PERIODS) {
			/* ringing should be gone by now */
			state = WAITING;
			RECEIVE_INT_ENABLE();
			SET_SENSOR_RECEIVE();
			periods++;
		}
		else
			periods++;
		break;

		case WAITING:
		if(periods > TIMEOUT_PERIODS) {
			/* we've waited too long, they're not coming */
			RECEIVE_INT_DISABLE();
			state = IDLE;
		}
		else
			periods++;
		break;

		default:
		break;
	}
}


/* this interrupt is called when transducer output changes state */
ISR(INT_RECEIVE) {
	/* TODO: add some timing inteligence to filter out random spikes */
	echo_tics = (periods * SENSOR_TCNT_MAX + SENSOR_TCNT) << SENSOR_TIME_DIV;
	RECEIVE_INT_DISABLE();
	SENSOR_TIMER_INT_DISABLE();
	state = IDLE;
}
