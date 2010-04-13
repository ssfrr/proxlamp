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
	IGNORING,
	RECEIVING,
	RECOVERING
} state_t;

static state_t state = IDLE;
static volatile uint16_t periods;
static volatile uint32_t echo_tics = 0;
static volatile uint16_t echo_periods = 0;
static uint8_t selected_sensor = 0;

#define TIMEOUT_TICS TIMEOUT_US * (F_CPU / 1000000UL)

#define TICS_PER_PERIOD SENSOR_TCNT_MAX >> SENSOR_TIME_DIV

#define TIMEOUT_PERIODS TIMEOUT_TICS / TICS_PER_PERIOD

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
	periods = 0;
	TRIGGER_SENSOR(selected_sensor);
	SENSOR_TIMER_RESET();
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
	periods++;
	if(state == PULSING && periods > 51) {
		RELEASE_SENSOR(selected_sensor);
		state = IGNORING;
	}
	else if(state == IGNORING && periods > 85) {
		RECEIVE_INT_CLEARFLAG();
		RECEIVE_INT_ENABLE();
		state = LISTENING;
	}
	else if(periods > 500) {
		RECEIVE_INT_DISABLE();
		SENSOR_TIMER_INT_DISABLE();
		CLR_TEST_PIN();
		echo_tics = 0;
		state = IDLE;
	}
}

/* this interrupt is called when transducer output changes state */
ISR(INT_RECEIVE) {
	echo_tics = (periods * SENSOR_TCNT_MAX + SENSOR_TCNT) 
		<< SENSOR_TIME_DIV;
	echo_periods = periods;
	RECEIVE_INT_DISABLE();
	SENSOR_TIMER_INT_DISABLE();
	state = IDLE;
	CLR_TEST_PIN();
}
