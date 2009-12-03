#include "proxlamp_r01_bsp.h"
#include <avr/interrupt.h>
#include <avr/io.h>
void bsp_setup() {
	clock_setup();
	pin_io_setup();
	sei();
}

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

