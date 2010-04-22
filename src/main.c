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

#define min(x,y) ((x) < (y) ? (x) : (y))
#define diff(x,y) min((x)-(y),(y)-(x))

#define DISTANCE_READINGS 5

int main(void) {
	uint16_t target_brightness = 0;
	uint16_t current_brightness = 0;
	uint16_t distances[DISTANCE_READINGS];
	uint8_t distance_index = 0;
	uint16_t distance_avg;
	uint16_t distance_current; 
	uint16_t distance_prev = 0;

	uint8_t exact_count = 0;
	uint16_t distance_min = 510;
	uint16_t distance_max = 1500;
	/* set up board-specific stuff */
	bsp_setup();
	
	//select_sensor(0);

	start_reading();
	while(1) {
		if(!sensor_busy()) {
			distance_current = get_distance();
			if(diff(distance_prev,distance_current) < 100) {
				uint8_t i;
				uint32_t sum = 0;
				if(distance_prev == distance_current) {
					exact_count++;
					if(exact_count > 5)
						distance_max = distance_current;
				}
				else
					exact_count = 0;
				
				distances[distance_index] = distance_current;
				distance_index = (distance_index + 1) % DISTANCE_READINGS;
				for(i = 0; i != DISTANCE_READINGS; i++)
					sum += distances[i];
				distance_avg = sum / DISTANCE_READINGS;
				if(distance_avg < distance_min)
					target_brightness = UINT16_MAX;
				else if(distance_avg > distance_max)
					target_brightness = 0;
				else
					target_brightness = UINT16_MAX  - (distance_avg-distance_min) * (UINT16_MAX / (distance_max-distance_min));
			}
			distance_prev = distance_current;

			start_reading();
		}
		if(target_brightness != current_brightness)
			//current_brightness += (int16_t)(target_brightness - current_brightness)/100;
			current_brightness += (target_brightness > current_brightness ? 1 : -1);
		set_brightness(current_brightness);
		_delay_us(200);
	}
	return 0;
}

