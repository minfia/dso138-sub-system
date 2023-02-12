/**
 * @file    dso138.h
 * @brief   DSO138デバイスヘッダ
 *
 * @author  minfia
 */


#ifndef INCLUDE_GUARD_UUID_717E2EF1_838F_433E_93D8_6DBB7C526BE5
#define INCLUDE_GUARD_UUID_717E2EF1_838F_433E_93D8_6DBB7C526BE5


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @brief モードのステート
 */
typedef enum {
    DSO138_MODE_STATE_TIME_BASE,            //!< 時間軸切り替え
    DSO138_MODE_STATE_CHANGE_EDGE,          //!< エッジ検知選択
    DSO138_MODE_STATE_CHANGE_TRIG_LEVEL,    //!< トリガーレベル
    DSO138_MODE_STATE_HORIZONTAL_POS,       //!< 時間軸移動
    DSO138_MODE_STATE_VERTICAL_POS,         //!< 波形移動
    DSO138_MODE_STATE_TRIG_AUTO,            //!< 自動波形更新
    DSO138_MODE_STATE_TRIG_NORMAL,          //!< エッジ検知時のみ波形更新
    DSO138_MODE_STATE_TRIG_SIGNAL,          //!< トリガー待ち

    DSO138_MODE_STATE_MAX
} DSO138_MODE_STATE;


/**
 * @brief   DSO138物理ボタンアクセスポート設定
 */
typedef struct {
    volatile uint8_t *port; //!< ポート名
    uint8_t num;            //!< ポート番号
} DSO138_KEY_t;

/**
 * @brief   DSO138物理ボタンアクセス
 */
typedef struct {
    DSO138_KEY_t ok;    //!< OKボタン
    DSO138_KEY_t plus;  //!< +ボタン
    DSO138_KEY_t minus; //!< -ボタン
    DSO138_KEY_t sel;   //!< SELボタン
} DSO138_BUTTON_t;

/**
 * @brief   初期化
 */
void dso138_init(DSO138_BUTTON_t button);

/**
 * @brief   動作ステートの変更
 *
 * @param [in]  next_state  変更するステート
 */
void dso138_change_state(DSO138_MODE_STATE next_state);

/**
 * @brief   現在の動作ステートの取得
 *
 * @return  DSO138_MODE_STATE;
 */
DSO138_MODE_STATE dso138_get_state(void);

/**
 * @brief   波形の保存
 */
void dso138_save_wave(void);

/**
 * @brief   波形の読み出し
 */
void dso138_load_wave(void);


/**
 * @brief   DSO138のOKボタン押下
 */
void dso138_press_ok_button(void);

/**
 * @brief   DSO138のOKボタン開放
 */
void dso138_release_ok_button(void);

/**
 * @brief   DSO138の+ボタン単押し
 *
 * @param [in]  push_cnt    繰り返し回数
 */
void dso138_enter_plus_key(uint8_t push_cnt);

/**
 * @brief   DSO138の-ボタン単押し
 *
 * @param [in]  push_cnt    繰り返し回数
 */
void dso138_enter_minus_key(uint8_t push_cnt);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* INCLUDE_GUARD_UUID_717E2EF1_838F_433E_93D8_6DBB7C526BE5 */
