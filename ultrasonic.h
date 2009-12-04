#ifndef ULTRASONIC_H
#define UlTRASONIC_H

/* 
 * impulse near receiver (hand clap) causes the receiver
 * to resonate at 41kHz, maximum amplitude around 150mV P-P.
 */

/* transmit at 41 kHz */
#define FTRANS 41000
/* calculate the counter value */
#define SENSOR_PULSE ((FOSC / FTRANS / 2) >> SENSOR_TIME_DIV)
/* how many pulses to send each time */
#define NUM_PULSES 20
#define TWICE_NUM_PULSES (NUM_PULSES << 1)

void select_sensor(unsigned char sensor);
void send_pulses(unsigned int pulses);

#endif /* ULTRASONIC_H */
