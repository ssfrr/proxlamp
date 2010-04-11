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

/* 
 * Sensor Sequence:
 *
 * 1. Initiate reading by bringing sensor trigger pin high, start counting time 
 * 2. Wait for sensor output pin to go low, indicating end of
 *    pulse transmission
 * 3. Bring sensor trigger line low
 * 4. Wait for sensor output pin to go high, indicating echo receipt
 * 5. store counter value, wait for recovery time
 * 6. idle state
 *
 */

typedef enum {
	IDLE,
	PULSING,
	LISTENING,
	RECEIVING,
	RECOVERING
} state_t;

state_t state = IDLE;
static volatile uint16_t periods;
static volatile uint32_t echo_tics = 0;
static volatile uint16_t echo_periods = 0;
static selected_sensor = 0;

#define TIMEOUT_TICS TIMEOUT_US * F_CPU / 1000000


#define TICS_PER_PERIOD SENSOR_TCNT_MAX >> SENSOR_TIME_DIV

#define UM_PER_PERIOD 5332

/* return the distance in mm */
uint16_t get_distance(void) {
	//return (uint16_t)((echo_tics * UINT16_MAX) / TIMEOUT_TICS);
	return (uint16_t)((uint32_t)echo_periods * UM_PER_PERIOD / 1000);
}

/* set up the hardware for the proper sensor */
void select_sensor(uint8_t sensor) {
	SENSOR_SELECT(sensor);
	selected_sensor = sensor;
}

/* return whether or not the sensor is ready to start another reading */
uint8_t sensor_busy() {
	return (state != IDLE);
}

/* 
 * start a sensor measurement, begin counting, and start waiting for 
 * end of transmission 
 */
void start_reading(void) {
	if(sensor_busy())
		return;
	SET_TEST_PIN();
	state = PULSING;
	SENSOR_COMP = SENSOR_TCNT_MAX;
	SENSOR_TCNT = 0;
	periods = 0;
	/* clear any existing sensor timer flags */
	SENSOR_TIMER_INT_CLEARFLAG();
	/* enable sensor timer interrupt */
	SENSOR_TIMER_INT_ENABLE();
	RECEIVE_INT_ENABLE();
}

/* 
 * this interrupt is triggered every time the sensor counter hits
 * SENSOR_COMP. The counter is automatically set to 0.
 */
ISR(INT_SENSOR_TIMER) {
	periods++;
	switch(state) {

		case PULSING:
		case LISTENING:
		case RECEIVING:
		default:
		break;
	}
}

/* if we hit this interrupt, then we didn't get another pulse in time */
ISR(INT_PULSE_COUNTER) {
	state = LISTENING;
	PULSE_COUNTER_INT_DISABLE();
}


/* this interrupt is called when transducer output changes state */
ISR(INT_RECEIVE) {
	static unsigned char received_edges;

	unsigned char pulse_tics = PULSE_COUNTER_TCNT;
	PULSE_COUNTER_TCNT = 0;

	switch(state) {
		case PULSING:
			if(!TEST_RECEIVE_PIN())
				CLR_SENSOR_PIN(sensor);
			/* first pulse of a group */
			break;

		case RECEIVING:
			/* we've recently received another pulse, check how long ago */
			if(pulse_tics < PULSE_TICS_MIN) {
				/* pulse was too short, reset and wait for another */
				state = LISTENING;
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
				echo_periods = periods;
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
