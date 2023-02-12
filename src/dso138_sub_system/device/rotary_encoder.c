/**
 * @file    rotary_encoder.c
 * @brief   ロータリーエンコーダデバイスドライバ
 *
 * @author  minfia
 */ 

#include "rotary_encoder.h"


volatile static ROTARY_ENCODER_ROTATE_STATE rotate_state;   //!< 回転方向
volatile static uint8_t pin_history;                        //!< A相B相の状態保持用


/**
 * @brief   1クリック分のビット遷移と回転方向構造体
 */
typedef struct {
    uint8_t state_val;
    ROTARY_ENCODER_ROTATE_STATE state_name;
} ROTARY_DIRECTION_t;

/**
 * @brief   正転/逆転の情報
 */
 static const ROTARY_DIRECTION_t rotary_directions[] = {
    { 0b10000111, ROTARY_ENCODER_ROTATE_STATE_CW  },
    { 0b01001011, ROTARY_ENCODER_ROTATE_STATE_CCW }
};


void rotary_enc_init(void)
{
    rotate_state = ROTARY_ENCODER_ROTATE_STATE_NO_CHANGE;
    pin_history = 0;
}

void rotary_enc_regist_pin(uint8_t pin)
{
    pin_history <<= 2;
    pin_history |= pin;
}

void rotary_enc_calc_state(void)
{
    for (int i=0; i<sizeof(rotary_directions)/sizeof(rotary_directions[0]); i++) {
        if (rotary_directions[i].state_val == pin_history) {
            pin_history = 0;
            rotate_state = rotary_directions[i].state_name;
            return;
        }
    }
}

ROTARY_ENCODER_ROTATE_STATE rotary_enc_get_state(void)
{
    ROTARY_ENCODER_ROTATE_STATE state = rotate_state;
    rotate_state = ROTARY_ENCODER_ROTATE_STATE_NO_CHANGE;

    return state;
}
