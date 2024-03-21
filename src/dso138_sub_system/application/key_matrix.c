/**
 * @file    key_matrix.c
 * @brief   キーマトリクスアプリケーション
 *
 * @author minfia
 */


#include "key_matrix.h"
#include <avr/io.h>
#include "../driver/timer.h"
#include "../device/key_matrix.h"


static INPUT_KEY_TYPE input_key;


static INPUT_KEY_TYPE key_proc(READ_KEYS_t keys)
{
    if (0b0001 & keys.keys) {
        switch (keys.sw_num) {
            case KEY_SCAN_NUM_0:
                return INPUT_KEY_TYPE_0_0;
            case KEY_SCAN_NUM_1:
                return INPUT_KEY_TYPE_1_0;
            case KEY_SCAN_NUM_2:
                return INPUT_KEY_TYPE_2_0;
            default:
                break;
        }
    } else if (0b0010 & keys.keys) {
        switch (keys.sw_num) {
            case KEY_SCAN_NUM_0:
                return INPUT_KEY_TYPE_0_1;
            case KEY_SCAN_NUM_1:
                return INPUT_KEY_TYPE_1_1;
            case KEY_SCAN_NUM_2:
                return INPUT_KEY_TYPE_2_1;
            default:
                break;
        }
    } else if (0b0100 & keys.keys) {
        switch (keys.sw_num) {
            case KEY_SCAN_NUM_0:
                return INPUT_KEY_TYPE_0_2;
            case KEY_SCAN_NUM_1:
                return INPUT_KEY_TYPE_1_2;
            case KEY_SCAN_NUM_2:
                return INPUT_KEY_TYPE_2_2;
            default:
                break;
        }
    }

    return INPUT_KEY_TYPE_NONE;
}


void app_key_matrix_init(void)
{
    timer0_init();
    KEY_SCAN_KEY_TYPE_t o[KEY_SCAN_NUM_MAX];
    o[0].port = &PORTB;
    o[0].num = 0;
    o[1].port = &PORTB;
    o[1].num = 1;
    o[2].port = &PORTB;
    o[2].num = 2;
    key_matrix_init(o, KEY_SCAN_NUM_MAX, 3);
    input_key = INPUT_KEY_TYPE_NONE;
}

void app_key_matrix_poll(void)
{
    timer0_stop();
    READ_KEYS_t keys = key_matrix_scan(PINB, 3, 3);
    timer0_resume();
    input_key = key_proc(keys);
}

INPUT_KEY_TYPE app_key_matrix_get_key_type(void)
{
    INPUT_KEY_TYPE tmp = input_key;
    input_key = INPUT_KEY_TYPE_NONE;
    return tmp;
}
