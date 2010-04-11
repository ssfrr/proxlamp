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


/* let the transducer ringdown for this many microseconds */
#define RINGDOWN_US 1500

/* stop listening to echos after this many microseconds */
#define TIMEOUT_US 20000


uint8_t sensor_busy(void);
uint16_t get_distance(void);
void select_sensor(uint8_t sensor);
void start_reading(void);

#endif /* ULTRASONIC_H */
