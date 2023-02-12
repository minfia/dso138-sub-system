/**
 * @file timer.c
 * @brief TIMERドライバ
 *
 * @author minfia
 */ 

#include "timer.h"
#include <avr/interrupt.h>


/**
 * @brief   タイマー0の割り込み処理ハンドラ
 */
__attribute__ ((weak)) void timer0_event_handler(void)
{
}


/**
 * @brief   タイマー0一致割り込み
 */
ISR(TIMER0_COMP_vect)
{
    timer0_event_handler();
}


void timer0_init(void)
{
    // 10ms = 1/100
    // 100 `=. 8000000 / 1024 / 78
    TCCR0 |= 0b00001000;
    TIMSK |= 0b00000000;
    OCR0 = 78;
    TCNT0 = 0;
}

void timer0_start(void)
{
    TCNT0 = 0;
    TIMSK |= (0b1 << OCIE0);
    TCCR0 |= 0b111; // clock divider is 1024 clk
}

void timer0_stop(void)
{
    TIMSK &= ~(0b1 << OCIE0);
    TCCR0 &= 0b11111000;
}

void timer0_resume(void)
{
    TIMSK |= (0b1 << OCIE0);
    TCCR0 |= 0b111; // clock divider is 1024 clk
}
