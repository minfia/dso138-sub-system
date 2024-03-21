/**
 * @file    dso138.c
 * @brief   DSO138デバイスドライバ
 *
 * @author  minfia
 */ 

#include "dso138.h"
#include <stdint.h>
#include "../driver/port.h"
#include "../driver/systick.h"
#include "../driver/eeprom.h"


#define DSO138_BTN_PUSH_TIME 100            ///< DSO138のボタン押下時間[ms]


/**
 * @brief   エッジ検知方向
 */
typedef enum {
    DSO138_EDGE_TYPE_RISE,  ///< 立ち上がり
    DSO138_EDGE_TYPE_FALL,  ///< 立ち下がり

    DSO138_EDGE_TYPE_MAX
} DSO138_EDGE_TYPE;

/**
 * @brief   トリガー選択
 */
typedef enum {
    DSO138_TRIG_TYPE_AUTO,      ///< 常時更新
    DSO138_TRIG_TYPE_NORMAL,    ///< 信号が来てるときのみ更新
    DSO138_TRIG_TYPE_SIGNAL,    ///< ワンショットトリガーで更新

    DSO138_TRIG_TYPE_MAX
} DSO138_TRIG_TYPE;

/**
 * @brief   DSO138のSELボタンで切り替わる状態
 */
typedef enum {
    DSO138_SEL_BTN_STATE_TIME_BASE,         ///< 時間軸切り替え
    DSO138_SEL_BTN_STATE_TRIG_SELECT,       ///< トリガー選択
    DSO138_SEL_BTN_STATE_CHANGE_EDGE,       ///< エッジ検知選択
    DSO138_SEL_BTN_STATE_CHANGE_TRIG_LEVEL, ///< トリガーレベル
    DSO138_SEL_BTN_STATE_HORIZONTAL_POS,    ///< 時間軸移動
    DSO138_SEL_BTN_STATE_VERTICAL_POS,      ///< 波形移動

    DSO138_SEL_BTN_STATE_MAX
} DSO138_SEL_BTN_STATE;

/**
 * @brief   DSO138への設定情報
 */
typedef struct {
    DSO138_MODE_STATE state;        //!< 現在の選択状態
    DSO138_SEL_BTN_STATE btn_state; //!< 現在のSELの選択状態
    DSO138_EDGE_TYPE edge;          //!< 現在のエッジ検出状態
    DSO138_TRIG_TYPE trig;          //!< 現在のトリガー選択状態
} DSO138_STATUS_t;


DSO138_STATUS_t dso138_status;  //!< DSO138の状態保持
DSO138_BUTTON_t dso138_button;  //!< DSO138ボタン設定


/**
 * @brief   時間待ち
 *
 * @param [in]  ms  時間(ms)
 */
static void delay_ms(const uint32_t ms)
{
    uint32_t tick = get_systick();
    while (1) {
        if (ms <= (get_systick() - tick)) return;
    }
}

/**
 * @brief   DSO138のOKボタン押下
 */
static void press_ok_key(void)
{
    port_set(dso138_button.ok.port, dso138_button.ok.num);
}

/**
 * @brief   DSO138のOKボタン開放
 */
static void release_ok_key(void)
{
    port_clear(dso138_button.ok.port, dso138_button.ok.num);
}

/**
 * @brief   DSO138のOKボタン単押し
 *
 * @param [in]  push_cnt    繰り返し回数
 */
static void enter_ok_key(uint8_t push_cnt)
{
    for (uint8_t i=0; i<push_cnt; i++) {
        press_ok_key();
        delay_ms(DSO138_BTN_PUSH_TIME);
        release_ok_key();
        delay_ms(DSO138_BTN_PUSH_TIME);
    }
}

/**
 * @brief   DSO138の+ボタン押下
 */
static void press_plus_key(void)
{
    port_set(dso138_button.plus.port, dso138_button.plus.num);
}

/**
 * @brief   DSO138の+ボタン開放
 */
static void release_plus_key(void)
{
    port_clear(dso138_button.plus.port, dso138_button.plus.num);
}

/**
 * @brief   DSO138の-ボタン押下
 */
static void press_minus_key(void)
{
    port_set(dso138_button.minus.port, dso138_button.minus.num);
}

/**
 * @brief   DSO138の-ボタン開放
 */
static void release_minus_key(void)
{
    port_clear(dso138_button.minus.port, dso138_button.minus.num);
}

/**
 * @brief   DSO138のSELボタン押下
 */
static void press_sel_key(void)
{
    port_set(dso138_button.sel.port, dso138_button.sel.num);
}

/**
 * @brief   DSO138のSELボタン開放
 */
static void release_sel_key(void)
{
    port_clear(dso138_button.sel.port, dso138_button.sel.num);
}

/**
 * @brief   DSO138のSELボタン単押し
 *
 * @param [in]  push_cnt    繰り返し回数
 */
static void enter_sel_key(uint8_t push_cnt)
{
    for (uint8_t i=0; i<push_cnt; i++) {
        press_sel_key();
        delay_ms(DSO138_BTN_PUSH_TIME);
        release_sel_key();
        delay_ms(DSO138_BTN_PUSH_TIME);
    }
}

/**
 * @brief   TimeBaseの切り替え処理
 *
 * @param [in]  push_cnt    切り替わるまでの押下回数
 */
static void process_time_base(uint8_t push_cnt)
{
    enter_sel_key(push_cnt);
}

/**
 * @brief   エッジ検出の切り替え処理
 *
 * @param [in]  push_cnt    切り替わるまでの押下回数
 */
static DSO138_EDGE_TYPE process_change_edge(uint8_t push_cnt)
{
    enter_sel_key(push_cnt);
    if (DSO138_EDGE_TYPE_RISE == dso138_status.edge) {
        // 立ち下がりへ
        dso138_enter_minus_key(1);
        return DSO138_EDGE_TYPE_FALL;
    } else {
        // 立ち上がりへ
        dso138_enter_plus_key(1);
        return DSO138_EDGE_TYPE_RISE;
    }
}

/**
 * @brief   トリガーレベルの切り替え処理
 *
 * @param [in]  push_cnt    切り替わるまでの押下回数
 */
static void process_change_trig_level(uint8_t push_cnt)
{
    enter_sel_key(push_cnt);
}

/**
 * @brief   電圧位置の切り替え処理
 *
 * @param [in]  push_cnt    切り替わるまでの押下回数
 */
static void process_horizontal_pos(uint8_t push_cnt)
{
    enter_sel_key(push_cnt);
}

/**
 * @brief   時間位置の切り替え処理
 *
 * @param [in]  push_cnt    切り替わるまでの押下回数
 */
static void process_vertical_pos(uint8_t push_cnt)
{
    enter_sel_key(push_cnt);
}

/**
 * @brief   トリガーモードの切り替え処理
 *
 * @param [in]  push_cnt    切り替わるまでの押下回数
 * @param [in]  trig        切り替えるトリガータイプ
 */
static void process_trig_mode(uint8_t push_cnt, DSO138_TRIG_TYPE trig)
{
    enter_sel_key(push_cnt);
    switch (dso138_status.trig) {
        case DSO138_TRIG_TYPE_AUTO:
            if (DSO138_TRIG_TYPE_NORMAL == trig) {
                dso138_enter_plus_key(1);
            } else if (DSO138_TRIG_TYPE_SIGNAL == trig) {
                dso138_enter_plus_key(2);
            }
            break;
        case DSO138_TRIG_TYPE_NORMAL:
            if (DSO138_TRIG_TYPE_AUTO == trig) {
                dso138_enter_minus_key(1);
            } else if (DSO138_TRIG_TYPE_SIGNAL == trig) {
                dso138_enter_plus_key(1);
            }
            break;
        case DSO138_TRIG_TYPE_SIGNAL:
            if (DSO138_TRIG_TYPE_AUTO == trig) {
                dso138_enter_minus_key(2);
            } else if (DSO138_TRIG_TYPE_NORMAL == trig) {
                dso138_enter_minus_key(1);
            } else {
                enter_ok_key(1);
            }
            break;
        default:
            break;
    }
}

/**
 * @brief   DSO138の現在のステートから、目的のステートまでの数を取得
 *
 * @param [in]  now_state   現在のDSO138のSELボタンのステート
 * @param [in]  new_state   目的のDSO138のSELボタンのステート
 *
 * @return  ステート数
 */
static uint8_t get_state_range(DSO138_SEL_BTN_STATE now_state, DSO138_SEL_BTN_STATE next_state)
{
    if (now_state == next_state) return 0;

    return (now_state < next_state) ? (next_state - now_state) : ((DSO138_SEL_BTN_STATE_MAX - now_state) + next_state);
}

/**
 * @brief   目的のステートから、DSO138のSELボタンに該当するステートを取得
 *
 * @param [in]  next_state  目的のステート
 *
 * @return  DSO138のSELボタンステート
 */
static DSO138_SEL_BTN_STATE get_next_sel_btn_state(DSO138_MODE_STATE next_state)
{
    switch (next_state) {
        case DSO138_MODE_STATE_TIME_BASE:
            return DSO138_SEL_BTN_STATE_TIME_BASE;
        case DSO138_MODE_STATE_TRIG_AUTO:
        case DSO138_MODE_STATE_TRIG_NORMAL:
        case DSO138_MODE_STATE_TRIG_SIGNAL:
            return DSO138_SEL_BTN_STATE_TRIG_SELECT;
        case DSO138_MODE_STATE_CHANGE_EDGE:
            return DSO138_SEL_BTN_STATE_CHANGE_EDGE;
        case DSO138_MODE_STATE_CHANGE_TRIG_LEVEL:
            return DSO138_SEL_BTN_STATE_CHANGE_TRIG_LEVEL;
        case DSO138_MODE_STATE_HORIZONTAL_POS:
            return DSO138_SEL_BTN_STATE_HORIZONTAL_POS;
        case DSO138_MODE_STATE_VERTICAL_POS:
            return DSO138_SEL_BTN_STATE_VERTICAL_POS;
        default:
            return DSO138_SEL_BTN_STATE_MAX;
    }
}

/**
 * @brief   DSO138の状態遷移制御
 *
 * @param [in]  state           目的のステート
 * @param [in]  sel_push_cnt    SELボタンの押下回数
 *
 */
static void process_dso138_ctrl(DSO138_MODE_STATE state, uint8_t sel_push_cnt)
{
    switch (state) {
        case DSO138_MODE_STATE_TIME_BASE:
            process_time_base(sel_push_cnt);
            break;
        case DSO138_MODE_STATE_TRIG_AUTO:
            process_trig_mode(sel_push_cnt, DSO138_TRIG_TYPE_AUTO);
            dso138_status.trig = DSO138_TRIG_TYPE_AUTO;
            break;
        case DSO138_MODE_STATE_TRIG_NORMAL:
            process_trig_mode(sel_push_cnt, DSO138_TRIG_TYPE_NORMAL);
            dso138_status.trig = DSO138_TRIG_TYPE_NORMAL;
            break;
        case DSO138_MODE_STATE_TRIG_SIGNAL:
            process_trig_mode(sel_push_cnt, DSO138_TRIG_TYPE_SIGNAL);
            dso138_status.trig = DSO138_TRIG_TYPE_SIGNAL;
            break;
        case DSO138_MODE_STATE_CHANGE_EDGE:
            dso138_status.edge = process_change_edge(sel_push_cnt);
            break;
        case DSO138_MODE_STATE_CHANGE_TRIG_LEVEL:
            process_change_trig_level(sel_push_cnt);
            break;
        case DSO138_MODE_STATE_HORIZONTAL_POS:
            process_horizontal_pos(sel_push_cnt);
            break;
        case DSO138_MODE_STATE_VERTICAL_POS:
            process_vertical_pos(sel_push_cnt);
            break;
        default:
            break;
    }
    eeprom_write(EEPROM_PARAM_DSO138_EDGE_DIR, dso138_status.edge);
    eeprom_write(EEPROM_PARAM_DSO138_TRIG, dso138_status.trig);
}


void dso138_init(DSO138_BUTTON_t button)
{
    dso138_button = button;
    dso138_status.state = DSO138_MODE_STATE_TIME_BASE;
    eeprom_read(EEPROM_PARAM_DSO138_EDGE_DIR, &dso138_status.edge);
    eeprom_read(EEPROM_PARAM_DSO138_TRIG, &dso138_status.trig);
}

void dso138_change_state(DSO138_MODE_STATE next_state)
{
    DSO138_SEL_BTN_STATE btn_next_state = get_next_sel_btn_state(next_state);
    if (DSO138_SEL_BTN_STATE_MAX == btn_next_state) return;

    uint8_t push_cnt = get_state_range(dso138_status.btn_state, btn_next_state);
    /*
     * Edge Change以外かつ、TRIG(Auto - Signal)以外かつ移動量が0
     */
    if ((DSO138_MODE_STATE_CHANGE_EDGE != next_state) &&
        ((DSO138_MODE_STATE_TRIG_AUTO >= next_state) && (next_state >= DSO138_MODE_STATE_TRIG_SIGNAL)) &&
        (0 == push_cnt)) return;

    process_dso138_ctrl(next_state, push_cnt);

    dso138_status.state = next_state;
    dso138_status.btn_state = btn_next_state;
}

DSO138_MODE_STATE dso138_get_state(void)
{
    return dso138_status.state;
}

void dso138_save_wave(void)
{
    press_sel_key();
    press_plus_key();
    delay_ms(DSO138_BTN_PUSH_TIME);
    release_sel_key();
    release_plus_key();
    delay_ms(DSO138_BTN_PUSH_TIME);
}

void dso138_load_wave(void)
{
    press_sel_key();
    press_minus_key();
    delay_ms(DSO138_BTN_PUSH_TIME);
    release_sel_key();
    release_minus_key();
    delay_ms(DSO138_BTN_PUSH_TIME);
}


void dso138_press_ok_button(void)
{
    press_ok_key();
}

void dso138_release_ok_button(void)
{
    release_ok_key();
}

void dso138_enter_plus_key(uint8_t push_cnt)
{
    for (uint8_t i=0; i<push_cnt; i++) {
        press_plus_key();
        delay_ms(DSO138_BTN_PUSH_TIME);
        release_plus_key();
        delay_ms(DSO138_BTN_PUSH_TIME);
    }
}

void dso138_enter_minus_key(uint8_t push_cnt)
{
    for (uint8_t i=0; i<push_cnt; i++) {
        press_minus_key();
        delay_ms(DSO138_BTN_PUSH_TIME);
        release_minus_key();
        delay_ms(DSO138_BTN_PUSH_TIME);
    }
}
