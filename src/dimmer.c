/*********************************************
 *
 * dimmer.c
 * 
 * Synchronizes to zero-crossings of the incoming AC cycle. At each
 * zero-crossing it waits some fraction of the half-wavelength and then
 * triggers a TRIAC to enable current to flow to a lightbulb
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
#include "dimmer.h"
#include <avr/interrupt.h>
#include <avr/io.h>

/* cycles to hold the triac high for triggering */
const uint16_t TRIGGER_TIME = 1E-4 * F_CPU;
volatile uint32_t wait_time;

/* 8.3ms in my apartment @ 8MHz = 66400 cycles 
 * it's rounded down a bit to make sure it doesn't wait all the way
 * until the next cycle*/
/* TODO: set halfcycle dynamically */
uint32_t ac_halfcycle = 65500;

/* Function Definitions */
void set_brightness(uint16_t brightness) {
	/* don't let brightness get all the way to 0xFFFF, because
	 * the TRIAC won't have time to trigger */
	/* TODO: turn on triac immediately for small wait times*/
	if(brightness > 0xFFF4)
		brightness = 0xFFF4;
	wait_time = (UINT16_MAX - (uint32_t)brightness) * (uint32_t)ac_halfcycle / UINT16_MAX;
}

/* schedule a timer interrupt in cycles */
inline void set_triac_alarm(uint32_t cycles) {
	TRIAC_TCNT = TRIAC_TCNT_MAX - (cycles >> TRIAC_TIME_DIV);
}

/* zero-crossing interrupt handler */

ISR(INT_ZEROCROSS) {
	/* sample 3 more times to check for noise */
	char val = ZEROCROSS_PINS & ZEROCROSS_PIN;
	int n = 3;
	while(n--)
		if((ZEROCROSS_PINS & ZEROCROSS_PIN) != val)
			return;
	TRIAC_PORT &= ~TRIAC_PIN;
	set_triac_alarm(wait_time);

	/* write a 1 to clear any pending timer interrupts */
	TRIAC_TIFR |= TRIAC_INT_FLAG;

	/* enable overflow interrupt for timer1 */
	TRIAC_TIMSK |= TRIAC_INT_EN;

	/* disable interrupt from INT0 so we don't re-trigger */
	ZEROCROSS_EIMSK &= ~ZEROCROSS_INT_EN;
}

/* Triac Timer interrupt handler */
ISR(INT_TRIAC_TIMER) {
	if(TRIAC_PORT & TRIAC_PIN) {
		TRIAC_PORT &= ~TRIAC_PIN;
		/* disable overflow interrupt for timer1 */
		TRIAC_TIMSK &= ~TRIAC_INT_EN;

		/* write a 1 to clear any pending external interrupts */
		ZEROCROSS_EIFR |= ZEROCROSS_INT_FLAG;
		/* enable interrupt from INT0, wait for next zero-crossing */
		ZEROCROSS_EIMSK |= ZEROCROSS_INT_EN;
	}
	else {
		TRIAC_PORT |= TRIAC_PIN;
		set_triac_alarm(TRIGGER_TIME);
	}
}
