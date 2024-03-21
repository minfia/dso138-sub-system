/**
 * @file key_matrix.h
 * @brief キーマトリクスライブラリ
 *
 * @author minfia
 */


#ifndef INCLUDE_GUARD_UUID_128BBA20_0A65_41C5_9692_638C853D97D4
#define INCLUDE_GUARD_UUID_128BBA20_0A65_41C5_9692_638C853D97D4


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#define INPUT_SIZE_MAX  3   //!< キー入力数(Column)


/**
 * @brief   キースキャン設定
 */
typedef struct {
    volatile uint8_t *port; //!< ポート名
    uint8_t num;            //!< ポート番号
} KEY_SCAN_KEY_TYPE_t;

/**
 * @brief   キースキャンの数(Row)
 */
typedef enum {
    KEY_SCAN_NUM_0,
    KEY_SCAN_NUM_1,
    KEY_SCAN_NUM_2,

    KEY_SCAN_NUM_MAX,
    KEY_SCAN_NUM_NONE
} KEY_SCAN_NUM;

/**
 * @brief   キー状態取得構造体
 */
typedef struct {
    KEY_SCAN_NUM sw_num;    //!< キースキャン位置
    uint8_t keys;           //!< 検出したキー
} READ_KEYS_t;


/**
 * @brief   キーマトリクスの初期化
 *
 * @param [in]  output_list  キースキャン設定
 * @param [in]  output_size  キースキャン設定数
 * @param [in]  input_size   キー入力数
 */
void key_matrix_init(KEY_SCAN_KEY_TYPE_t *output_list, const uint8_t output_size, const uint8_t input_size);

/**
 * @brief   キー入力を取得
 *
 * @param [in]  port_input  スキャンするポート名
 * @param [in]  scan_size   スキャンするビット数
 * @param [in]  shift_bit   右シフトするビット数
 *
 * @return  キースキャン位置と入力状態の構造体
 */
READ_KEYS_t key_matrix_scan(uint8_t port_input, uint8_t scan_size, uint8_t shift_bit);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* INCLUDE_GUARD_UUID_128BBA20_0A65_41C5_9692_638C853D97D4 */
