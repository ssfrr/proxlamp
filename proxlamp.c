#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* Pin and Port Definitions */
#define TRIAC_PORT PORTD
#define TRIAC_DD DDRD
#define TRIAC_PIN (1 << PD0)

#define SENSOR_PORT PORTB
#define SENSOR_DD DDRB
#define SENSOR_PINS PINB

#define ZEROCROSS_PORT PORTD
#define ZEROCROSS_DD DDRD
#define ZEROCROSS_PINS PIND
#define ZEROCROSS_PIN (1 << PD2)

#define TEST_PORT PORTD
#define TEST_DD DDRD
#define TEST_PIN (1 << PD1)

/* Global Variables */

/* cycles to hold the triac high for triggering */
const uint16_t TRIGGER_TIME = 1E-4 * F_CPU;
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

}

void pin_io_setup(void) {
	/* set the triac pin to output */
	TRIAC_DD |= TRIAC_PIN; 
	/* start in low state */
	TRIAC_PORT &= ~TRIAC_PIN; 

	/* set the test pin to output */
	TEST_DD |= TEST_PIN; 
	/* start in low state */
	TEST_PORT &= ~TEST_PIN; 

	/* set the zero cross detector pin to input */
	ZEROCROSS_DD &= ~ZEROCROSS_PIN; 
	/* disable pullup resistor on zero-cross detector */
	ZEROCROSS_PORT &= ~ZEROCROSS_PIN;

	/* set the sensor port to all input */
	SENSOR_DD = 0x00;
	/* enable pull-up resistors on sensor input */
	SENSOR_PORT = 0xFF;

	/* set up INT0 interrupt pin to trigger on both edges */
	EICRA = 0x01;
	/* enable interrupt from INT0 */
	EIMSK = 0x01;
}

/* schedule a timer interrupt in cycles */
inline void set_alarm1(uint32_t cycles) {
	TCNT1 = UINT16_MAX - (cycles / 8);
}

/* INT0 zero-crossing interrupt handler */

ISR(INT0_vect) {
	/* sample 5 more times to check for noise */
	char val = ZEROCROSS_PINS & ZEROCROSS_PIN;
	int n = 50;
	while(n--)
		if((ZEROCROSS_PINS & ZEROCROSS_PIN) != val)
			return;
	TEST_PORT ^= TEST_PIN;
	TRIAC_PORT &= ~TRIAC_PIN;
	set_alarm1(pulse_width);

	/* clear any pending timer interrupts */
	TIFR1 |= 1 << TOV1;

	/* enable overflow interrupt for timer1 */
	TIMSK1 = 0x01;

	/* disable interrupt from INT0 so we don't re-trigger */
	EIMSK = 0x00;
}

/* timer1 overflow interrupt handler */
ISR(TIMER1_OVF_vect) {
	if(TRIAC_PORT & TRIAC_PIN) {
		TRIAC_PORT &= ~TRIAC_PIN;
		/* disable overflow interrupt for timer1 */
		TIMSK1 = 0x00;

		/* clear any pending external interrupts */
		EIFR |= 1 << INTF0;
		/* enable interrupt from INT0, wait for next zero-crossing */
		EIMSK = 0x01;
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

	clock_setup();
	pin_io_setup();
	
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

