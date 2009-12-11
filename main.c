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
#include "sched.h"

state_t state = IDLE;
extern volatile unsigned int echo_tics;

int main(void) {
	/* PULSING CODE
	int brightness_period = 2000;
	int elapsed_ms = 0;
	*/

	/* set up board-specific stuff */
	bsp_setup();
	
	select_sensor(0);

	while(1) {
		/* PULSING CODE
		if(elapsed_ms < brightness_period) {
			set_brightness((float)elapsed_ms / brightness_period);
		}
		else if(elapsed_ms < 2 * brightness_period) {
			set_brightness(1 - (float)(elapsed_ms - brightness_period) / 
				brightness_period);
		}
		else 
			elapsed_ms = 0;
		elapsed_ms++;

		_delay_ms(1);
		*/

		send_pulses(10);
		_delay_ms(50);
	}
	return 0;
}

