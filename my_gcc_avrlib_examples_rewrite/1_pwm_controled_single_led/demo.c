/*
 * -----------------------------------------------------------------------------
 * " THE BEER-WARE LICENSE" (Revision 42):
 * <joerg@FreeBSD.org> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.	joerg Wunsch
 * -----------------------------------------------------------------------------
 *
 * Simple AVR demostration. COntrols a LED that can be directly
 * conneted from OC1/OC1A to GND. The brightness of the LED is
 * conrolled with the PWM. After each period of the PWM, the PWM
 * value is either incremented or decremented, that's all.
 *
 * $Id: group__demo__project.html, v1.1.1.19 2010/08/11 13:41:36 joerg_wunsch Exp $
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "iocompat.h"

enum {UP, DOWN};

ISR (TIMER1_OVF_vect)
{
    static uint16_t pwm;
    static uint8_t direction;

    switch (direction) {
    case UP:
	if (++pwm == TIMER1_TOP)
	    direction = DOWN;
	break;
    case DOWN:
	if (--pwm == 0)
	    direction = UP;
	break;
    }

    OCR = pwm;
}

void
ioinit (void)
{
    /* timer 1 is 10-bit PWM(8-bit PWM on some ATtinys). */
    TCCR1A = TIMER1_PWM_INIT;
    /*
     * start timer1.
     *
     * NB: TCCR1A and TCCR1B could actually be the same register, so
     * take care to not clobber it.
     */
    TCCR1B |= TIMER1_CLOCKSOURCE;
    /*
     * Run any device-dependent timer 1 setup hook if present.
     */
#if defined (TIMER1_SETUP_HOOK)
    TIMER1_SETUP_HOOK();
#endif

    /* Set PWM value to 0. */
    OCR = 0;

    /* Enable OC1 as output. */
    DDROC = _BV(OC1);

    /* Enable timer 1 overflow interrupt. */
    TIMSK = _BV(TOIE1);

    sei();
}

int
main (void)
{
    ioinit();
    /* loop forever, the interrupts are doing the rest. */

    for (;;)
	sleep_mode();

    return 0;
}
