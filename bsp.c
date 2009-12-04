#include "proxlamp_r01_bsp.h"
#include <avr/interrupt.h>
#include <avr/io.h>
void bsp_setup() {
	clock_setup();
	triac_timer_setup();
	sensor_timer_setup();
	pin_io_setup();
	/* enable interupts */
	sei();
}

void clock_setup(void) {
	/* set the main clock prescaler to 1 */
	CLKPR = 1 << CLKPCE;
	CLKPR = 0;
}

void triac_timer_setup() {
	/* run counter in normal mode */
	TRIAC_CONF_A = 0x00;
	/* set up 16-bit counter with prescalar of 8 */
	TRIAC_CONF_B = (1 << CS11);
}

void sensor_timer_setup() {
	/* run counter in clear-on-compare mode */
	SENSOR_CONF_A = (1 << WGM01);
	/* set up 8-bit counter with no prescalar */
	SENSOR_CONF_B = (1 << CS00);
	SENSOR_COMP = SENSOR_TCNT_MAX;
	/* disable timer interrupts for this counter */
	SENSOR_TIMSK &= ~SENSOR_INT_EN
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

