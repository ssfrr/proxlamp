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

/* Registers and Bitmasks */

/* Registers for Triac Timing */
#define TRIAC_TIMSK TIMSK1
#define TRIAC_TIFR TIFR1
#define TRIAC_TCNT TCNT1

/* Masks for Triac Timer Interrupts */
#define TRIAC_INT_FLAG (1 << TOV1)
#define TRIAC_INT_EN (1 << TOIE1)

/* Registers for Zerocross External Interrupt */
#define ZEROCROSS_EIMSK EIMSK
#define ZEROCROSS_EIFR EIFR

/* Masks for Zerocross External Interrupt */
#define ZEROCROSS_INT_FLAG (1 << INTF0)
#define ZEROCROSS_INT_EN (1 << INT0) 

/* Registers for sensor timing */

/* Board-Specific Constants */
#define TRIAC_TCNT_MAX UINT16_MAX
#define TRIAC_TIME_DIV 3

/* Board-Specific  Setup Functions */
void bsp_setup();

void clock_setup();
void pin_io_setup();

#endif /* PROXLAMP_BSP_H */
