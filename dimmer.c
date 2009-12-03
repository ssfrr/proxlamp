#include "bsp.h"
#include "dimmer.h"
#include <avr/interrupt.h>
#include <avr/io.h>

/* cycles to hold the triac high for triggering */
const uint16_t TRIGGER_TIME = 1E-4 * F_CPU;
volatile uint32_t pulse_width;

/* measure the half-cycle duration in cycles */
/* 8.3ms in my apartment @ 8MHz = 66400 cycles */
/* TODO: set halfcycle dynamically */
uint32_t ac_halfcycle = 66400;

/* Function Definitions */
void set_brightness(float brightness) {
	pulse_width = 0.95 * brightness * ac_halfcycle;
}

/* schedule a timer interrupt in cycles */
inline void set_triac_alarm(uint32_t cycles) {
	TRIAC_TCNT = TRIAC_TCNT_MAX - (cycles >> TRIAC_TIME_DIV);
}

/* zero-crossing interrupt handler */

ISR(INT_ZEROCROSS) {
	/* sample 5 more times to check for noise */
	char val = ZEROCROSS_PINS & ZEROCROSS_PIN;
	int n = 50;
	while(n--)
		if((ZEROCROSS_PINS & ZEROCROSS_PIN) != val)
			return;
	TEST_PORT ^= TEST_PIN;
	TRIAC_PORT &= ~TRIAC_PIN;
	set_triac_alarm(pulse_width);

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
