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

typedef enum {
	IDLE,
	PULSING,
	IGNORING,
	WAITING,
	RECEIVING
} state_t;

state_t state = IDLE;
static volatile unsigned int periods;
volatile unsigned int echo_tics = 0;

/* calculate the counter value for the transmit frequency */
#define PULSE_TICS ((F_CPU / FTRANS / 2) >> SENSOR_TIME_DIV)

/* calculate the maximum and minmum thresholds to recieve pulses */
#define PULSE_TICS_MAX (PULSE_TICS * 3) >> 1
#define PULSE_TICS_MIN PULSE_TICS >> 1

#define RINGDOWN_PERIODS (RINGDOWN_US * (F_CPU / 1000000) \
		/ (SENSOR_TCNT_MAX >> SENSOR_TIME_DIV))

#define TIMEOUT_PERIODS (TIMEOUT_US* (F_CPU / 1000000) \
		/ (SENSOR_TCNT_MAX >> SENSOR_TIME_DIV))

#define TIMEOUT_TICS TIMEOUT_US * F_CPU / 1000000


/* return the last valid distance as 16-bit unsigned int 
 * 65536 represents the furthest distance, set by TIMEOUT_US */
inline unsigned int get_distance() {
	return (unsigned int)(((unsigned long)echo_tics * UINT16_MAX) / TIMEOUT_TICS);
}

/* set up the hardware for the proper sensor */
inline void select_sensor(unsigned char sensor) {
	SENSOR_SELECT(sensor);
}

/* return whether or not the sensor is ready to start another reading */
inline unsigned char sensor_busy() {
	return (state != IDLE);
}

/* send a number of ultrasonic pulses at FTRANS Hz */
void send_pulses(unsigned int pulses) {
	if(sensor_busy())
		return;
	state = PULSING;
	/* set the timer compare value to the pulse length */
	SENSOR_COMP = PULSE_TICS;
	/* we're actually counting half-cycles */
	periods = (pulses << 1) + 1;
	SET_SENSOR_SEND();
	SENSOR_TCNT = 0;
	/* clear any existing sensor timer flags */
	SENSOR_TIMER_INT_CLEARFLAG();
	/* enable sensor timer interrupt */
	SENSOR_TIMER_INT_ENABLE();
	SET_TEST_PIN();
}

/* 
 * this interrupt is triggered every time the sensor counter hits
 * SENSOR_COMP. The counter is automatically set to 0.
 */
ISR(INT_SENSOR_TIMER) {
	switch(state) {

		case PULSING:
		if(--periods) {
			/* while we still have pulses to send */
			TOGGLE_PULSE_PIN();
		}
		else {
			/* now we wait a little longer for the resonance to die down*/
			state = IGNORING;
			/* we're going to count how many overflows occur */
			SENSOR_COMP = SENSOR_TCNT_MAX;
		}
		break;

		case IGNORING:
		if(periods >= RINGDOWN_PERIODS) {
			/* ringing should be gone by now */
			state = WAITING;
			RECEIVE_INT_ENABLE();
			periods++;
		}
		else
			periods++;
		break;

		case WAITING:
		case RECEIVING:
		if(periods >= TIMEOUT_PERIODS) {
			/* we've waited too long, they're not coming */
			RECEIVE_INT_DISABLE();
			SENSOR_TIMER_INT_DISABLE();
			state = IDLE;
			CLR_TEST_PIN();
		}
		else
			periods++;
		break;

		default:
		break;
	}
}

/* if we hit this interrupt, then we didn't get another pulse in time */
ISR(INT_PULSE_COUNTER) {
	state = WAITING;
	PULSE_COUNTER_INT_DISABLE();
}


/* this interrupt is called when transducer output changes state */
ISR(INT_RECEIVE) {
	static unsigned char received_edges;

	unsigned char pulse_tics = PULSE_COUNTER_TCNT;
	PULSE_COUNTER_TCNT = 0;
	SET_TEST_PIN();

	switch(state) {
		case WAITING:
			/* first pulse of a group */
			PULSE_COUNTER_COMP = PULSE_TICS_MAX;
			received_edges = 1;
			state = RECEIVING;
			PULSE_COUNTER_INT_ENABLE();
			break;

		case RECEIVING:
			/* we've recently received another pulse, check how long ago */
			if(pulse_tics < PULSE_TICS_MIN) {
				/* pulse was too short, reset and wait for another */
				state = WAITING;
				PULSE_COUNTER_INT_DISABLE();
			}
			else {
				/* pulse was the right length */
				received_edges++;
			}
			if(received_edges >= 3) {
				/* we've received 3 proper-length pulses in a row, it's an echo */
				echo_tics = (periods * SENSOR_TCNT_MAX + SENSOR_TCNT) 
					<< SENSOR_TIME_DIV;
				RECEIVE_INT_DISABLE();
				PULSE_COUNTER_INT_DISABLE();
				SENSOR_TIMER_INT_DISABLE();
				state = IDLE;
				CLR_TEST_PIN();
			}
			break;
		default:
			break;
	}
}
