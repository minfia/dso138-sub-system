/**
 * @file    systick.c
 * @brief   system tick
 *
 * @author  minfia
 */


#include "systick.h"
#include <avr/interrupt.h>
#include <stdint.h>


volatile static uint32_t system_timer = 0;


ISR(TIMER1_COMPA_vect)
{
    ++system_timer;
}

void systick_init()
{
    // setting to 1ms
    TCCR1A = 0;
    TCCR1B |= (1 << WGM12);
    TCCR1C = 0;
    TIMSK1 |= (1 << OCIE1A);

    OCR1A = 499;
    TCNT1 = 0;

    TCCR1B |= 0b010;
}

uint32_t get_systick()
{
    cli();
    uint32_t temp = system_timer;
    sei();
    return temp;
}
