#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* Pin and Port Definitions */
#define TRIAC_PORT PORTD
#define TRIAC_DD DDRD
#define TRIAC_PIN (1 << PD0)

#define SENSOR_PORT PORTB

#define ZEROCROSS_PORT PORTD
#define ZEROCROSS_PIN (1 << PD2)

/* Global Variables */

/* measure the half-cycle duration in cycles */
uint32_t ac_halfcycle = F_CPU / 120;
volatile uint32_t pulse_width;


/* Function Definitions */
void clock_setup(void) {
	/* set the main clock prescaler to 1 */
	CLKPR = 1 << CLKPCE;
	CLKPR = 0;

	/* TODO: use register bit names for portability */
	/* set up 16-bit counter with prescalar of 8 */
	TCCR1A = 0x00;
	TCCR1B = 0x02;
	/* enable overflow interrupt for timer1 */
	TIMSK1 = 0x01;

	/* enable interrupts */
	sei();
}

void pin_io_setup(void) {
	TRIAC_DD |= TRIAC_PIN; /* set the triac pin to output */
	TRIAC_PORT |= TRIAC_PIN;
}

/* schedule a timer interrupt in cycles */
inline void set_alarm1(uint32_t cycles) {
	TCNT1 = UINT16_MAX - (cycles / 8) + TCNT1;
}

/* timer1 overflow interrupt handler */
ISR(TIMER1_OVF_vect) {
	if(TRIAC_PORT & TRIAC_PIN) {
		TRIAC_PORT &= ~TRIAC_PIN;
		set_alarm1(ac_halfcycle - pulse_width);
	}
	else {
		TRIAC_PORT |= TRIAC_PIN;
		set_alarm1(pulse_width);
	}
}

int main(void) {
	clock_setup();
	pin_io_setup();
	
	float brightness = 0;
	int brightness_period = 1000;
	int elapsed_ms = 0;

	/* kickstart timer */
	TCNT1 = UINT16_MAX - 1;

	while(1) {
		if(elapsed_ms < brightness_period) {
			brightness = (float)elapsed_ms / brightness_period;
			if(brightness > 0.99)
				brightness = 0.99;
			pulse_width = brightness * ac_halfcycle;
		}
		else if(elapsed_ms < 2 * brightness_period) {
			brightness = 1 - (float)(elapsed_ms - brightness_period) / 
				brightness_period;
			if(brightness > 0.99)
				brightness = 0.99;
			pulse_width = brightness * ac_halfcycle;
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

