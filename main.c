/* include BSP header before delay.h for clock definition */
#include "bsp.h"
#include <util/delay.h>
#include "dimmer.h"
#include "ultrasonic.h"
#include "sched.h"

state_t state = IDLE;
char sensor = 0;

int main(void) {
	int brightness_period = 2000;
	int elapsed_ms = 0;

	/* set up board-specific stuff */
	bsp_setup();
	
	while(1) {
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
	}
	return 0;
}

