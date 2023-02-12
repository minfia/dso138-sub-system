/**
 * @file    rotary_encoder.h
 * @brief   ロータリーエンコーダデバイスドライバヘッダ
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
 * @brief   ロータリエンコーダの回転完了状態
 */
typedef enum {
    ROTARY_ENCODER_ROTATE_STATE_NO_CHANGE,  //!< 変更なし
    ROTARY_ENCODER_ROTATE_STATE_CW,         //!< 正転
    ROTARY_ENCODER_ROTATE_STATE_CCW,        //!< 逆転
} ROTARY_ENCODER_ROTATE_STATE;


/**
 * @brief   ロータリーエンコーダ初期化
 */
void rotary_enc_init(void);

/**
 * @brief   現在のピン状態を状態変化用に登録
 *
 * @param [in]  pin     A,B相のピン状態
 * @details
 *  - pinは0bit目はA相, 1bit目はB相の状態を入れる 
 */
void rotary_enc_regist_pin(uint8_t pin);

/**
 * @brief   ロータリーエンコーダの回転状態を計算
 */
void rotary_enc_calc_state(void);

/**
 * @brief   ロータリーエンコーダの回転状態を取得
 *
 * @return  ロータリエンコーダの回転完了状態
 */
ROTARY_ENCODER_ROTATE_STATE rotary_enc_get_state(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* INCLUDE_GUARD_UUID_1BEE0193_7F85_4D4B_BE4E_5202044E2F5C */
