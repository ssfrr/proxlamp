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
 *
 * noise floor seems to be around 60mV P-P, with a 6mV DC offset
 */

/* transmit at 39.5 kHz */
#define FTRANS 39500

/* let the transducer ringdown for this many microseconds */
#define RINGDOWN_US 2500

/* stop listening to echos after this many microseconds */
#define TIMEOUT_US 12000


inline unsigned char sensor_busy();
inline unsigned int get_distance();
inline void select_sensor(unsigned char sensor);
void send_pulses(unsigned int pulses);

#endif /* ULTRASONIC_H */
