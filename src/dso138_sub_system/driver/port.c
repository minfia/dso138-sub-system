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
 * @brief   ロータリーエンコーダのピン変化割り込み処理(A相)
 */
ISR(INT4_vect)
{
    rotary_enc_regist_pin((PINE >> PINE4) & 0b11);
}

/**
 * @brief   ロータリーエンコーダのピン変化割り込み処理(B相)
 */
ISR(INT5_vect)
{
    rotary_enc_regist_pin((PINE >> PINE4) & 0b11);
}


void port_init()
{
    /*
        ポート設定
        キーマトリクス
          Output
            PORTA[2:0]
          Input
            PINA[6:4]
        DSO138のスイッチ
          PB0: OK
          PB1: +
          PB2: -
          PB3: SEL
        DSO138の仮想スイッチ
          PINA7: OK
        ロータリーエンコーダ
          PE4: A相
          PE5: B相
        設定リセット
          PINA3
    */
    DDRA = 0x07;
    PORTA = 0xF8;
    DDRB = 0x0F;
    PORTB = 0x00;
    DDRE = 0x00;
    PORTE = 0xFF;

    // interrupt setting
    EIMSK = 0x30; // Enable INT4,5
    EICRB = 0x05; // Edge both INT4,5
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
