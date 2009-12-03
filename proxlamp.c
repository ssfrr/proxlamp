/* include BSP header before delay.h for clock definition */
#include "proxlamp_r01_bsp.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* Global Variables */

/* cycles to hold the triac high for triggering */
const uint16_t TRIGGER_TIME = 1E-4 * F_CPU;
volatile uint32_t pulse_width;


/* Function Definitions */
/* schedule a timer interrupt in cycles */
inline void set_alarm1(uint32_t cycles) {
	TRIAC_TCNT = TRIAC_TCNT_MAX - (cycles >> TRIAC_TIME_DIV);
}

/* INT0 zero-crossing interrupt handler */

ISR(INT_ZEROCROSS) {
	/* sample 5 more times to check for noise */
	char val = ZEROCROSS_PINS & ZEROCROSS_PIN;
	int n = 50;
	while(n--)
		if((ZEROCROSS_PINS & ZEROCROSS_PIN) != val)
			return;
	TEST_PORT ^= TEST_PIN;
	TRIAC_PORT &= ~TRIAC_PIN;
	set_alarm1(pulse_width);

	/* write a 1 to clear any pending timer interrupts */
	TRIAC_TIFR |= TRIAC_INT_FLAG;

	/* enable overflow interrupt for timer1 */
	TRIAC_TIMSK |= TRIAC_INT_EN;

	/* disable interrupt from INT0 so we don't re-trigger */
	ZEROCROSS_EIMSK &= ~ZEROCROSS_INT_EN;
}

/* timer1 overflow interrupt handler */
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
		set_alarm1(TRIGGER_TIME);
	}
}

int main(void) {
	float brightness = 0;

	/* measure the half-cycle duration in cycles */
	/* 8.3ms in my apartment @ 8MHz = 66400 cycles */
	uint32_t ac_halfcycle = 66400;

	int brightness_period = 10000;
	int elapsed_ms = 0;

	/* set up board-specific stuff */
	bsp_setup();
	
	/* enable interrupts */
	sei();

	while(1) {
		if(elapsed_ms < brightness_period) {
			brightness = (float)elapsed_ms / brightness_period;
			pulse_width = 0.95 * brightness * ac_halfcycle;
			//if(pulse_width > ac_halfcycle - TRIGGER_TIME - 100)
			//	pulse_width = ac_halfcycle - TRIGGER_TIME - 100;
		}
		else if(elapsed_ms < 2 * brightness_period) {
			brightness = 1 - (float)(elapsed_ms - brightness_period) / 
				brightness_period;
			pulse_width = 0.95 * brightness * ac_halfcycle;
			//if(pulse_width > ac_halfcycle - TRIGGER_TIME - 100)
				//pulse_width = ac_halfcycle - TRIGGER_TIME - 100;
		}
		else 
			elapsed_ms = 0;
		elapsed_ms++;

		/*
		   if(brightness > 0.1)
		   TRIAC_PORT |= TRIAC_PIN;
		   else
		   TRIAC_PORT &= ~TRIAC_PIN;
		   */

		_delay_ms(1);
	}
	return 0;
}

