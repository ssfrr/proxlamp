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

int main(void) {
	/* set up board-specific stuff */
	bsp_setup();
	
	//select_sensor(0);

	while(1) {
		start_reading();
		while(sensor_busy()) {};
		uint16_t distance = get_distance();
		set_brightness(distance * 65);
		_delay_ms(500);
	}
	return 0;
}

