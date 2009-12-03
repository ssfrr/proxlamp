#include "bsp.h"
/*
1 cycle of 40 kHz: 25 us
at 8 MHz, that's 200 clock tics
*/

extern state_t state;
extern char sensor;

void send_pulse(int sensor) {
	state = PULSING;
	/* setup pwm to send 200 clock pulses */
}

