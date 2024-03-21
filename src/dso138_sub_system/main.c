/**
 * @file    main.c
 * @brief   main
 *
 * @author  minfia
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include "system.h"
#include "driver/systick.h"
#include "driver/port.h"
#include "driver/timer.h"
#include "driver/eeprom.h"
#include "device/rotary_encoder.h"
#include "device/key_matrix.h"
#include "device/dso138.h"
#include "application/key_matrix.h"


/**
 * @brief   �L�[���͂�DSO138�����̕R�Â�
 */
typedef struct {
    INPUT_KEY_TYPE key;                                 //!< �L�[����
    DSO138_MODE_STATE state;                            //!< DSO138�X�e�[�g
    void (*state_func)(DSO138_MODE_STATE next_state);   //!< �X�e�[�g�J�ڊ֐��|�C���^
    void (*option_func)(void);                          //!< �X�e�[�g�J�ڈȊO�̊֐��|�C���^
} KEY_DSO138_STATE_DIC;

static const KEY_DSO138_STATE_DIC key_state[] = {
    { INPUT_KEY_TYPE_0_0,   DSO138_MODE_STATE_CHANGE_EDGE,          dso138_change_state,    NULL                }, // SW3
    { INPUT_KEY_TYPE_0_1,   DSO138_MODE_STATE_TRIG_AUTO,            dso138_change_state,    NULL                }, // SW4
    { INPUT_KEY_TYPE_0_2,   DSO138_MODE_STATE_TRIG_SIGNAL,          dso138_change_state,    NULL                }, // SW5
    { INPUT_KEY_TYPE_1_0,   DSO138_MODE_STATE_VERTICAL_POS,         dso138_change_state,    NULL                }, // SW6
    { INPUT_KEY_TYPE_1_1,   DSO138_MODE_STATE_TIME_BASE,            dso138_change_state,    NULL                }, // SW7
    { INPUT_KEY_TYPE_1_2,   DSO138_MODE_STATE_CHANGE_TRIG_LEVEL,    dso138_change_state,    NULL                }, // SW8
    { INPUT_KEY_TYPE_2_0,   DSO138_MODE_STATE_HORIZONTAL_POS,       dso138_change_state,    NULL                }, // SW10
    { INPUT_KEY_TYPE_2_1,   DSO138_MODE_STATE_MAX,                  NULL,                   dso138_save_wave    }, // SW11
    { INPUT_KEY_TYPE_2_2,   DSO138_MODE_STATE_MAX,                  NULL,                   dso138_load_wave    }, // SW12
};


int main(void)
{
    systick_init();
    port_init();
    eeprom_init();
    rotary_enc_init();
    app_key_matrix_init();
    {
        DSO138_BUTTON_t button;
        button.ok.port = &PORTD;
        button.ok.num = 4;
        button.plus.port = &PORTD;
        button.plus.num = 5;
        button.minus.port = &PORTD;
        button.minus.num = 6;
        button.sel.port = &PORTD;
        button.sel.num = 7;
        dso138_init(button);
    }
    timer0_start();
    sei();

    while (1) {
        // �L�[���͏���
        if (0 == port_get(&PINC, 4)) {
            // �ݒ�L�����Z�b�g
            eeprom_reset();
        }
        if (0 == port_get(&PINC, 2)) {
            dso138_press_ok_button();
        } else {
            dso138_release_ok_button();
            app_key_matrix_poll();
            INPUT_KEY_TYPE key = app_key_matrix_get_key_type();
            for (int i=0; i<sizeof(key_state)/sizeof(KEY_DSO138_STATE_DIC); i++) {
                if (key_state[i].key == key) {
                    if ((DSO138_MODE_STATE_MAX != key_state[i].state) && (NULL != *key_state[i].state_func)) {
                        key_state[i].state_func(key_state[i].state);
                    } else if (NULL != *key_state[i].option_func) {
                        key_state[i].option_func();
                    }
                }
            }
            // ���[�^���[�G���R�[�_�̉�]�������擾
            ROTARY_ENCODER_ROTATE_STATE state = rotary_enc_get_state();
            /**
             * @note ���Ԏ��ړ��̂݃X�N���[���o�[�����[�^���[�G���R�[�_�̉�]�Ƌt�̓��������邽��\n
             *       ���Ԏ��ړ��ł�+��-�𔽓]������\n
             *       �܂��A�g���K�[���x���̏ꍇ1�񉟉��ł͈ړ����Ȃ�����2�񉟉����s��
             */
            if (ROTARY_ENCODER_ROTATE_STATE_CW == state) {
                switch (dso138_get_state()) {
                    case DSO138_MODE_STATE_CHANGE_TRIG_LEVEL:
                        dso138_enter_plus_key(2);
                        break;
                    default:
                        dso138_enter_plus_key(1);
                        break;
                }
            } else if (ROTARY_ENCODER_ROTATE_STATE_CCW == state) {
                switch (dso138_get_state()) {
                    case DSO138_MODE_STATE_CHANGE_TRIG_LEVEL:
                        dso138_enter_minus_key(2);
                        break;
                    default:
                        dso138_enter_minus_key(1);
                        break;
                }
            }
        }
    }
}
