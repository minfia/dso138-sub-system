/**
 * @file    port.c
 * @brief   IOポートドライバ
 *
 * @author  minfia
 */ 

#include "port.h"
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/interrupt.h>
#include "../device/rotary_encoder.h"


/**
 * @~japanese
 * @brief   ロータリーエンコーダのピン変化割り込み処理(A相/B相)
 * @~
 * @brief   Pin change interrupt acoording to rotary encoder in signal A and signal B
 */
ISR(PCINT1_vect)
{
    rotary_enc_regist_pin((PINC >> PINC0) & 0b11);
}


void port_init()
{
    /*
        port settings
        Key matrix
          Output
            PORTB[2:0]
          Input
            PINB[6:4]
        Control switch of DSO138
          PORTD4: OK
          PORTD5: +
          PORTD6: -
          PORTD7: SEL
        Virtual switch of DSO138
          PINC2: OK
        Rotary encoder
          PINC0:  Signal A
          PINC1:  Signal B
          PORTC3: LED
        Reset to EEPROM
          PINC4
    */
    DDRB = 0x07;
    PORTB = 0xF8;
    DDRD = 0xF0;
    PORTD = 0x0F;
    DDRC = 0x08;
    PORTC = 0xF7;

    // interrupt setting
    PCICR = 0x02;  // Enabled PCI1
    PCMSK1 = 0x03; // Enabled PCINT8,9
}

void port_set(volatile uint8_t *port, uint8_t num)
{
    *port |= _BV(num);
}

void port_clear(volatile uint8_t *port, uint8_t num)
{
    *port &= ~_BV(num);
}

uint8_t port_get(volatile uint8_t *port, uint8_t num)
{
    return ((*(port) >> num) & 0b1);
}
