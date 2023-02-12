/**
 * @file    key_matrix.h
 * @brief   キーマトリクスアプリケーションヘッダ
 *
 * @author minfia
 */


#ifndef INCLUDE_GUARD_UUID_17652776_0A3E_4685_AD6F_4BC4ABCF31F4
#define INCLUDE_GUARD_UUID_17652776_0A3E_4685_AD6F_4BC4ABCF31F4


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief   キー行列
 */
typedef enum {
    INPUT_KEY_TYPE_0_0, //!< OUTPUT:0, INPUT:0
    INPUT_KEY_TYPE_0_1, //!< OUTPUT:0, INPUT:1
    INPUT_KEY_TYPE_0_2, //!< OUTPUT:0, INPUT:2
    INPUT_KEY_TYPE_1_0, //!< OUTPUT:1, INPUT:0
    INPUT_KEY_TYPE_1_1, //!< OUTPUT:1, INPUT:1
    INPUT_KEY_TYPE_1_2, //!< OUTPUT:1, INPUT:2
    INPUT_KEY_TYPE_2_0, //!< OUTPUT:2, INPUT:0
    INPUT_KEY_TYPE_2_1, //!< OUTPUT:2, INPUT:1
    INPUT_KEY_TYPE_2_2, //!< OUTPUT:2, INPUT:2

    INPUT_KEY_TYPE_MAX,
    INPUT_KEY_TYPE_NONE
} INPUT_KEY_TYPE;

/**
 * @brief   キーマトリクスアプリケーションの初期化
 */
void app_key_matrix_init(void);

/**
 * @brief   キーマトリクス処理ポーリング
 */
void app_key_matrix_poll(void);

/**
 * @brief   入力キーの取得
 */
INPUT_KEY_TYPE app_key_matrix_get_key_type(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* INCLUDE_GUARD_UUID_17652776_0A3E_4685_AD6F_4BC4ABCF31F4 */
