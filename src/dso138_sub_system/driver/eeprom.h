/**
 * @file    eeprom.h
 * @brief   EEPROMドライバヘッダ
 *
 * @author  minfia
 */


#ifndef INCLUDE_GUARD_UUID_783E3878_8395_4B43_8A26_1429CE842C16
#define INCLUDE_GUARD_UUID_783E3878_8395_4B43_8A26_1429CE842C16


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @brief   EEPROMパラメータのアクセス名
 */
typedef enum {
    EEPROM_PARAM_DSO138_EDGE_DIR,   //!< DSO138のEdge方向
    EEPROM_PARAM_DSO138_TRIG,       //!< DSO138のトリガータイプ

    EEPROM_PARAM_MAX
} EEPROM_PARAM_TYPE;


/**
 * @brief   EEPROM初期化
 */
void eeprom_init(void);

/**
 * @brief   EEPROMからデータ取得
 * @param [in]      type    アクセス名
 * @param [out]     data    取得データ
 *
 * @note アクセス時間短縮のため、RAM上の値を返す
 */
void eeprom_read(const EEPROM_PARAM_TYPE type, uint8_t *data);

/**
 * @brief   EEPROMへデータ書き込み
 * @param [in]      type    アクセス名
 * @param [in]      data    書き込みデータ
 */
void eeprom_write(const EEPROM_PARAM_TYPE type, const uint8_t data);

/**
 * @brief   EEPROMのデータを初期化
 */
void eeprom_reset(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* INCLUDE_GUARD_UUID_783E3878_8395_4B43_8A26_1429CE842C16 */
