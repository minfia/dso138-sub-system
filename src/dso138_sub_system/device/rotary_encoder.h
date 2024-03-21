/**
 * @file    rotary_encoder.h
 * @~japanase
 * @brief   ロータリーエンコーダデバイスドライバヘッダ
 *
 * @~
 * @brief   rotary encoder device driver header file.
 *
 * @author  minfia
 */ 


#ifndef INCLUDE_GUARD_UUID_1BEE0193_7F85_4D4B_BE4E_5202044E2F5C
#define INCLUDE_GUARD_UUID_1BEE0193_7F85_4D4B_BE4E_5202044E2F5C


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @~japanese
 * @brief   ロータリエンコーダの回転完了状態
 *
 * @~
 * @brief   State for finished rotate in rotary encoder.
 */
typedef enum {
    /**
     * @~japanese
     * @brief   変化なし
     *
     * @~
     * @brief   Not changed
     */
    ROTARY_ENCODER_ROTATE_STATE_NO_CHANGE,
    /**
     * @~japanese
     * @brief   正転
     *
     * @~
     * @brief   Clock wise.
     */
    ROTARY_ENCODER_ROTATE_STATE_CW,
    /**
     * @~japanese
     * @brief   逆転
     *
     * @~
     * @brief   Counter clock wise.
     */
    ROTARY_ENCODER_ROTATE_STATE_CCW,
} ROTARY_ENCODER_ROTATE_STATE;


/**
 * @~japanese
 * @brief   ロータリーエンコーダ初期化
 *
 * @~
 * @brief   Initialize
 */
void rotary_enc_init(void);

/**
 * @~japanese
 * @brief   現在のピン状態を状態変化用に登録
 *
 * @param [in]  pin     A,B相のピン状態
 * @details
 *  - pinは0bit目はA相, 1bit目はB相の状態を入れる 
 *
 * @~
 * @brief   Regist to now pin state.
 *
 * @param [in]  pin     State of signal A and signal B.
 * @details
 *  - Regist to pin value is 0th bit to signal A, 1st bit to signal B.
 */
void rotary_enc_regist_pin(uint8_t pin);

/**
 * @~japanase
 * @brief   ロータリーエンコーダの回転状態を計算
 *
 * @~
 * @brief   Calcurate for rotate.
 */
void rotary_enc_calc_state(void);

/**
 * @~japanase
 * @brief   ロータリーエンコーダの回転状態を取得
 *
 * @return  ロータリエンコーダの回転完了状態
 *
 * @~
 * @brief   Get state of finished rotate.
 *
 * @return  State of finished rotate.
 */
ROTARY_ENCODER_ROTATE_STATE rotary_enc_get_state(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* INCLUDE_GUARD_UUID_1BEE0193_7F85_4D4B_BE4E_5202044E2F5C */
