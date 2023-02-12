/**
 * @file    eeprom.c
 * @brief   EEPROMドライバ
 *
 * @author  minfia
 */

#include "eeprom.h"
#include <avr/eeprom.h>
#include <stddef.h>


/**
 * @brief   EEPROMの名前とアドレス構造体
 */
typedef struct {
    EEPROM_PARAM_TYPE param;    //!< 外部からのアクセス名
    uint16_t addr;              //!< EEPROMのアドレス
} PARAM_ADDR_DIC;

/**
 * @brief   EEPROMデータ管理構造体
 */
typedef struct {
    PARAM_ADDR_DIC dic; //!< EEPROMの名前とアドレス
    uint8_t data;       //!< 現在のデータ
} DATA_PARAM;

DATA_PARAM eeprom_parameters[] = {
    { { EEPROM_PARAM_DSO138_EDGE_DIR,   0x0000 },   0 },
    { { EEPROM_PARAM_DSO138_TRIG,       0x0001 },   0 },
};

void eeprom_init(void)
{
    const size_t ary_size = sizeof(eeprom_parameters) / sizeof(eeprom_parameters[0]);
    for (size_t i=0; i<ary_size; i++) {
        eeprom_busy_wait();
        eeprom_parameters[i].data = eeprom_read_byte((uint8_t *)eeprom_parameters[i].dic.addr);
    }
}

void eeprom_read(const EEPROM_PARAM_TYPE type, uint8_t *data)
{
    const size_t ary_size = sizeof(eeprom_parameters) / sizeof(eeprom_parameters[0]);
    size_t i;
    for (i=0; i<ary_size; i++) {
        if (type == eeprom_parameters[i].dic.param) {
            break;
        }
    }
    if (ary_size <= i) {
        data = 0;
        return;
    }

    *data = eeprom_parameters[i].data;
}

void eeprom_write(const EEPROM_PARAM_TYPE type, const uint8_t data)
{
    const size_t ary_size = sizeof(eeprom_parameters) / sizeof(eeprom_parameters[0]);
    size_t i;
    for (i=0; i<ary_size; i++) {
        if (type == eeprom_parameters[i].dic.param) {
            break;
        }
    }
    if (ary_size <= i) return;
    DATA_PARAM *param = &eeprom_parameters[i];

    if (data == param->data) return;
    param->data = data;

    eeprom_busy_wait();
    eeprom_write_byte((uint8_t *)param->dic.addr, param->data);
}

void eeprom_reset(void)
{
    const size_t ary_size = sizeof(eeprom_parameters) / sizeof(eeprom_parameters[0]);
    for (size_t i=0; i<ary_size; i++) {
        DATA_PARAM *param = &eeprom_parameters[i];
        param->data = 0;
        eeprom_write_byte((uint8_t *)param->dic.addr, param->data);
    }
}
