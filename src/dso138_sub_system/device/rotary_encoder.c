/**
 * @file    rotary_encoder.c
 * @~japanase
 * @brief   ロータリーエンコーダデバイスドライバ
 * @~
 * @brief   rotary encoder device driver.
 *
 * @author  minfia
 */ 

#include "rotary_encoder.h"


/**
 * @~japanese
 * @brief   回転方向
 * @~
 * @brief   direction of rotate.
 */
volatile static ROTARY_ENCODER_ROTATE_STATE rotate_state;

/**
 * @~japanese
 * @brief   A相B相の状態保持用
 * @~
 * @brief   Value of hold to status in signal A and signal B.
 */
volatile static uint8_t pin_history;


/**
 * @~japanese
 * @brief   1クリック分のビット遷移と回転方向の構造体
 * @~
 * @brief   Structs for bit transition in 1 click and direction of rotate.
 */
typedef struct {
    uint8_t state_val;
    ROTARY_ENCODER_ROTATE_STATE state_name;
} ROTARY_DIRECTION_t;

/**
 * @~japanese
 * @brief   正転/逆転と1クリック分の情報
 * @~
 * @brief   Information of CW/CCW and 1 click bits.
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
