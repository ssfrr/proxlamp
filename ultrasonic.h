/*********************************************
 *
 * ultrasonic.h
 * 
 * Include this to interface with ultrasonic sensors
 * 
 *
 * Copyright 2009 Spencer Russell
 * Released under the GPLv3
 *
 * This file is part of The ProxLamp Firmware.
 *
 * The ProxLamp Firmware is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * The ProxLamp Firmware is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * The ProxLamp Firmware.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 ********************************************/



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
