/************************************
 *
 * proxlamp_r01_bsp.h
 *
 * This file defines all the pins, ports, and registers for Revision 01 of the
 * Proxlamp Hardware
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
 * **********************************/


/*
 * Pin Assignments:
 *
 * PD6			Pulse Generation	
 * PC(0-2)		Sensor Select
 * PC3			Sensor Send/Receive Select (0=send, 1=receive)
 * PD2(INT0)	AC Measurement(Zerocross Interrupt)
 * PD3(INT1)	Receiver Interrupt
 * PD0			Triac Control
 * PD1			General Test/Debug pin
 *
 * ISP:
 * PC6			Reset
 * PB3			MOSI
 * PB4			MISO
 * PB5			SCK
 */


#ifndef PROXLAMP_BSP_H
#define PROXLAMP_BSP_H

#define F_CPU 8000000UL
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

/* Interrupt Handlers */

#define INT_ZEROCROSS INT0_vect
#define INT_TRIAC_TIMER TIMER1_OVF_vect
#define INT_SENSOR_TIMER TIMER0_COMPA_vect

/* Registers and Bitmasks */

/* Registers for Triac Timing */
#define TRIAC_TIMSK TIMSK1
#define TRIAC_TIFR TIFR1
#define TRIAC_TCNT TCNT1
#define TRIAC_COMP OCR1A
#define TRIAC_CONF_A TCCR1A
#define TRIAC_CONF_B TCCR1B

/* Masks for Triac Timer Interrupts */
#define TRIAC_INT_FLAG (1 << TOV1)
#define TRIAC_INT_EN (1 << TOIE1)

/* Registers for Zerocross External Interrupt */
#define ZEROCROSS_EIMSK EIMSK
#define ZEROCROSS_EIFR EIFR

/* Masks for Zerocross External Interrupt */
#define ZEROCROSS_INT_FLAG (1 << INTF0)
#define ZEROCROSS_INT_EN (1 << INT0) 

/* Registers for Sensor pulse timer */
#define SENSOR_TIMSK TIMSK0
#define SENSOR_TIFR TIFR0
#define SENSOR_TCNT TCNT0
#define SENSOR_COMP OCR0Ah
#define SENSOR_CONF_A TCCR0A
#define SENSOR_CONF_B TCCR0B

/* Masks for Sensor Pulse Timer */
#define SENSOR_INT_FLAG (1 << OCF0A)
#define SENSOR_INT_EN (1 << OCIE0A)


/* Board-Specific Constants */
#define TRIAC_TCNT_MAX UINT16_MAX
#define SENSOR_TCNT_MAX UINT8_MAX
#define TRIAC_TIME_DIV 3
#define SENSOR_TIME_DIV 0

/* Board-Specific  Setup Functions */
void bsp_setup();

void clock_setup();
void triac_timer_setup();
void sensor_timer_setup();
void pin_io_setup();

#endif /* PROXLAMP_BSP_H */
