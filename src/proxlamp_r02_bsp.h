/************************************
 *
 * proxlamp_r02_bsp.h
 *
 * This file defines all the pins, ports, and registers for Revision 02 of the
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
 * PD4			Sensor 0 Trigger
 * PD5			Sensor 1 Trigger
 * PD6			Sensor 2 Trigger
 * PD7			Sensor 3 Trigger
 * PC(0-2)		Sensor Select
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

/* maybe we'll get fancy later:
typedef struct {
	unsigned select:	3;
	unsigned receive:	1;
	unsigned reserved:	4;
} SENSORbits;

#define SENSOR_SELECT (volatile SENSORbits *)(&PORTC)->select
#define SENSOR_RECEIVE (volatile SENSORbits *)(&PORTC)->receive
*/

#define F_CPU 8000000UL
#define NUM_SENSORS 4

/* Pin and Port Definitions */
#define TRIAC_PORT PORTD
#define TRIAC_DD DDRD
#define TRIAC_PIN (1 << PD0)
/* triac is active high */
#define SET_TRIAC() TRIAC_PORT |= TRIAC_PIN
#define CLR_TRIAC() TRIAC_PORT &= ~TRIAC_PIN

#define SENSOR_SEL_PORT PORTC
#define SENSOR_SEL_DD DDRC
#define SENSOR_SEL_MASK 0x07
#define SENSOR_SEL_SHIFT 0
#define SENSOR_SELECT(n) \
	do { \
	SENSOR_SEL_PORT = (SENSOR_SEL_PORT & ~SENSOR_SEL_MASK) | \
					((n << SENSOR_SEL_SHIFT) & SENSOR_SEL_MASK); \
	} while(0)

#define SENSOR_TRIGGER_PORT PORTD
#define SENSOR_TRIGGER_DD DDRD
#define SENSOR_TRIGGER_MASK (0x0F << 4)
#define CLR_TRIGGER_PINS() do { SENSOR_TRIGGER_PORT &= ~SENSOR_TRIGGER_MASK; } while(0)
#define SET_TRIGGER_PINS() do { SENSOR_TRIGGER_PORT |= SENSOR_TRIGGER_MASK; } while(0)
#define TRIGGER_SENSOR(n) do { SENSOR_TRIGGER_PORT |= (1 << (n+4)); } while(0)

#define RELEASE_SENSOR(n) do { SENSOR_TRIGGER_PORT &= !(1 << (n+4)); } while(0)

#define RECEIVE_PORT PORTD
#define RECEIVE_DD DDRD
#define RECEIVE_PINS PIND
#define RECEIVE_PIN (1 << PD3)
#define TEST_RECEIVE_PIN (RECEIVE_PORT & RECEIVE_PIN)

#define ZEROCROSS_PORT PORTD
#define ZEROCROSS_DD DDRD
#define ZEROCROSS_PINS PIND
#define ZEROCROSS_PIN (1 << PD2)

#define TEST_PORT PORTD
#define TEST_DD DDRD
#define TEST_PIN (1 << PD1)
#define SET_TEST_PIN() do { TEST_PORT |= TEST_PIN; } while(0)
#define CLR_TEST_PIN() do { TEST_PORT &= ~TEST_PIN; } while(0)

/* Interrupt Handlers */

#define INT_RECEIVE INT1_vect
#define INT_ZEROCROSS INT0_vect
#define INT_TRIAC_TIMER TIMER1_OVF_vect
#define INT_SENSOR_TIMER TIMER0_COMPA_vect

/* Registers and Bitmasks */

/* Registers for Triac Timing */
#define TRIAC_TIMSK TIMSK1
#define TRIAC_TIFR TIFR1
#define TRIAC_TCNT TCNT1
#define TRIAC_COMP OCR1A
#define TRIAC_CFG_A TCCR1A
#define TRIAC_CFG_B TCCR1B

/* Masks for Triac Timer Interrupts */
#define TRIAC_INT_FLAG (1 << TOV1)
#define TRIAC_INT_EN (1 << TOIE1)

/* Registers for Ultrasonic Receive External Interrupt */
#define RECEIVE_EIMSK EIMSK
#define RECEIVE_EIFR EIFR
#define RECEIVE_EICRA EICRA

/* Masks for Ultrasonic Receive External Interrupt */
#define RECEIVE_INT_FLAG (1 << INTF1)
#define RECEIVE_INT_EN (1 << INT1) 

#define RECEIVE_INT_CLEARFLAG() do { RECEIVE_EIFR |= RECEIVE_INT_FLAG; } while(0)
#define RECEIVE_INT_ENABLE() do { RECEIVE_EIMSK |= RECEIVE_INT_EN; } while(0)
#define RECEIVE_INT_DISABLE() do { RECEIVE_EIMSK &= ~RECEIVE_INT_EN; } while(0)

/* Registers for Zerocross External Interrupt */
#define ZEROCROSS_EIMSK EIMSK
#define ZEROCROSS_EIFR EIFR
#define ZEROCROSS_EICRA EICRA

/* Masks for Zerocross External Interrupt */
#define ZEROCROSS_INT_FLAG (1 << INTF0)
#define ZEROCROSS_INT_EN (1 << INT0) 

#define ZEROCROSS_INT_CLEARFLAG() do { ZEROCROSS_EIFR |= ZEROCROSS_INT_FLAG; } while(0)
#define ZEROCROSS_INT_ENABLE() do { ZEROCROSS_EIMSK |= ZEROCROSS_INT_EN; } while(0)
#define ZEROCROSS_INT_DISABLE() do { ZEROCROSS_EIMSK &= ~ZEROCROSS_INT_EN; } while(0)

/* Registers for Sensor timer */
#define SENSOR_TIMSK TIMSK0
#define SENSOR_TIFR TIFR0
#define SENSOR_TCNT TCNT0
#define SENSOR_COMP OCR0A
#define SENSOR_CFG_A TCCR0A
#define SENSOR_CFG_B TCCR0B

/* Masks for Sensor Timer */
#define SENSOR_INT_FLAG (1 << OCF0A)
#define SENSOR_INT_EN (1 << OCIE0A)

#define SENSOR_TIMER_INT_CLEARFLAG() do { SENSOR_TIFR |= SENSOR_INT_FLAG; } while(0)
#define SENSOR_TIMER_INT_ENABLE() do { SENSOR_TIMSK |= SENSOR_INT_EN; } while(0)
#define SENSOR_TIMER_INT_DISABLE() do { SENSOR_TIMSK &= ~SENSOR_INT_EN; } while(0)
#define SENSOR_TIMER_RESET() do { SENSOR_TCNT = 0; } while(0)

/* Board-Specific Constants */
#define TRIAC_TCNT_MAX UINT16_MAX
#define SENSOR_TCNT_MAX UINT8_MAX
#define PULSE_COUNTER_TCNT_MAX UINT8_MAX
#define TRIAC_TIME_DIV 3
#define SENSOR_TIME_DIV 0

#endif /* PROXLAMP_BSP_H */
