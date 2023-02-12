/**
 * @file    systick.c
 * @brief   system tick
 *
 * @author minfia
 */


#include "systick.h"
#include <avr/interrupt.h>
#include <stdint.h>


volatile static uint32_t system_timer = 0;


ISR(TIMER3_COMPA_vect)
{
    ++system_timer;
}

void systick_init()
{
    // 1msに設定
    TCCR3A = 0;
    TCCR3B |= (1 << WGM32);
    TCCR3C = 0;
    ETIMSK |= (1 << OCIE3A);

    OCR3A = 499;
    TCNT3 = 0;

    TCCR3B |= 0b010;
}

uint32_t get_systick()
{
    cli();
    uint32_t temp = system_timer;
    sei();
    return temp;
}
