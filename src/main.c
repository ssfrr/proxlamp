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


int main(void) {
	uint16_t target_brightness = 0;
	uint16_t current_brightness = 0;
	/* set up board-specific stuff */
	bsp_setup();
	
	//select_sensor(0);

	start_reading();
	while(1) {
		if(!sensor_busy()) {
			uint16_t distance = get_distance();
			if(distance < 510)
				target_brightness = UINT16_MAX;
			else if(distance > 1190)
				target_brightness = 0;
			else
				target_brightness = UINT16_MAX  - (distance-510) * 96;
			start_reading();
		}
		if(target_brightness != current_brightness)
			current_brightness += (target_brightness > current_brightness ? 1 : -1);
		set_brightness(current_brightness);
		_delay_us(200);
	}
	return 0;
}

