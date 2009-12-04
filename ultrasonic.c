#include "bsp.h"
#include <avr/interrupt.h>
#include "ultrasonic.h"
#include "sched.h"

volatile unsigned int periods;

int get_waiting_tics() {
	return (periods * SENSOR_TCNT_MAX + SENSOR_TCNT) << SENSOR_TIME_DIV;
}

void select_sensor(unsigned char sensor) {
	SENSORbits.SELECT = sensor;
}

void send_pulse(unsigned int pulses) {
	state = PULSING;
	/* set pin as output */
	SENSOR_DD = (1 << sensor);
	/* set the compare value to the pulse length */
	SENSOR_COMP = SENSOR_PULSE;
	/* we're actually counting half-cycles */
	periods = pulses << 1;
	SENSOR_TCNT = 0;
	/* clear any existing sensor timer flags */
	SENSOR_TIFR |= SENSOR_INT_FLAG;
	/* enable sensor timer interrupt */
	SENSOR_TIMSK |= SENSOR_INT_EN;
}

ISR(INT_SENSOR_TIMER) {
	if(state == PULSING) {
		if(periods--) {
			/* while we still have pulses to send */
			/* toggle the current sensor pin */
			SENSOR_PORT ^= (1 << sensor);
		}
		else {
			/* now we wait for the echo */
			state = WAITING;
			/* we're going to count how many overflows occur */
			SENSOR_COMP = SENSOR_TCNT_MAX;
		}
	}
	else if(state == WAITING) {
		periods++;
	}
}
