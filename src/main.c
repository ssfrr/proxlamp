/*********************************************
 *
 * main.c
 * 
 * Main Application Code for ProxLamp Firmware
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



/* include BSP header before delay.h for clock definition */
#include "bsp.h"
#include <util/delay.h>
#include "dimmer.h"
#include "ultrasonic.h"

/* pulse ranges from 3ms to 7ms 
 * or 1.02m to 2.38m (round trip)
 * or 0.51m to 1.19m (one way)
 *
 * difference: 680mm 
 * UINT_MAX / 680 = 96.37 */

#define MAX_DIST 1300
#define MIN_DIST 510
#define DIST_COEF (UINT16_MAX / (MAX_DIST - MIN_DIST))

#define DISTANCE_READINGS 30

int main(void) {
	uint16_t target_brightness = 0;
	uint16_t current_brightness = 0;
	uint16_t distances[DISTANCE_READINGS];
	uint8_t distance_index = 0;
	uint16_t distance_avg = 0;
	uint32_t sum;
	/* set up board-specific stuff */
	bsp_setup();
	
	//select_sensor(0);

	start_reading();
	while(1) {
		if(!sensor_busy()) {
			uint8_t i;
			distances[distance_index] = get_distance();
			distance_index = (distance_index + 1) % DISTANCE_READINGS;

			sum = 0;
			for(i = DISTANCE_READINGS-1; i != 0; i--)
				sum += distances[i];
			distance_avg = sum / DISTANCE_READINGS;
			if(distance_avg < MIN_DIST)
				target_brightness = UINT16_MAX;
			else if(distance_avg > MAX_DIST)
				target_brightness = 0;
			else
				target_brightness = UINT16_MAX  - (distance_avg-MIN_DIST) * DIST_COEF;
			start_reading();
		}
		if(target_brightness != current_brightness)
			current_brightness += (target_brightness > current_brightness ? 1 : -1);
		set_brightness(current_brightness);
		_delay_us(200);
	}
	return 0;
}

