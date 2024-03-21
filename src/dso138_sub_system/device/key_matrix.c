/**
 * @file key_matrix.c
 * @brief キーマトリクスライブラリ
 *
 * @author minfia
 */

#include "key_matrix.h"
#include "../driver/port.h"
#include "../driver/timer.h"


static volatile KEY_SCAN_KEY_TYPE_t output[KEY_SCAN_NUM_MAX];   //!< キースキャンポート保持用
static volatile KEY_SCAN_NUM output_num = KEY_SCAN_NUM_NONE;    //!< 現在のキースキャン位置
static volatile uint8_t old_input[KEY_SCAN_NUM_MAX];            //!< 前回のキー状態保持用
static uint8_t sw_input_mask;                                   //!< キー入力マスク


void timer0_event_handler(void)
{
    port_set(output[output_num].port, output[output_num].num);
    output_num++;
    if (KEY_SCAN_NUM_MAX <= output_num) output_num = KEY_SCAN_NUM_0;
    port_clear(output[output_num].port, output[output_num].num);
}


void key_matrix_init(KEY_SCAN_KEY_TYPE_t *output_list, const uint8_t output_size, const uint8_t input_size)
{
    if (KEY_SCAN_NUM_MAX < output_size) return;
    sw_input_mask = 0;
    for (uint8_t i=0; i<input_size; i++) {
        sw_input_mask |= 1 << i;
    }
    for (int i=0; i<output_size; i++) {
        output[i].port = output_list[i].port;
        output[i].num = output_list[i].num;
        port_set(output[i].port, output[i].num);
        old_input[i] = sw_input_mask;
    }
    output_num = KEY_SCAN_NUM_0;
    port_clear(output[0].port, output[0].num);
}

READ_KEYS_t key_matrix_scan(uint8_t port_input, uint8_t scan_size, uint8_t shift_bit)
{
    uint8_t now = (port_input >> shift_bit) & sw_input_mask; // 0 = ON, 1 = OFF
    READ_KEYS_t rkey = {0};
    uint8_t temp_num = output_num;
    if (KEY_SCAN_NUM_NONE == output_num) return rkey;
    rkey.sw_num  = (KEY_SCAN_NUM)temp_num;

    for (int i=0; i<scan_size; i++) {
        uint8_t bit = (1 << i);
        // 連続入力にならないようにトグル動作
        if ((0 == (bit & now)) && ((bit & now) != (bit & old_input[temp_num]))) {
            rkey.keys |= bit;
        }            
    }
    old_input[temp_num] = now;

    return rkey;
}
