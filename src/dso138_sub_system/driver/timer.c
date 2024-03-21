/**
 * @file    timer.c
 * @brief   TIMERドライバ
 *
 * @author  minfia
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
ISR(TIMER0_COMPA_vect)
{
    timer0_event_handler();
}


void timer0_init(void)
{
    // 10ms = 1/100
    // 100 `=. 8000000 / 1024 / 78
    TCCR0A |= (1 << WGM01);
    TIMSK0 = 0b00000000;
    OCR0A = 78;
    TCNT0 = 0;
}

void timer0_start(void)
{
    TCNT0 = 0;
    TIMSK0 |= (1 << OCIE0A);
    TCCR0B |= 0b101; // clock divider is 1024 clk
}

void timer0_stop(void)
{
    TIMSK0 &= ~(1 << OCIE0A);
    TCCR0B &= 0b11111000;
}

void timer0_resume(void)
{
    TIMSK0 |= (1 << OCIE0A);
    TCCR0B |= 0b101; // clock divider is 1024 clk
}
