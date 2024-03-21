/**
 * @file    systick.h
 * @brief   system tick
 *
 * @author  minfia
 */


#ifndef INCLUDE_GUARD_UUID_01111674_E090_43F0_8D74_9D696AFBE245
#define INCLUDE_GUARD_UUID_01111674_E090_43F0_8D74_9D696AFBE245


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @~japanese
 * @brief   systickタイマ初期化
 *
 * @~
 * @brief   Initialize systick
 */
void systick_init();

/**
 * @~japanese
 * @brief   現在のsystickを取得
 *
 * @~
 * @brief   Get systick
 */
uint32_t get_systick();


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* INCLUDE_GUARD_UUID_01111674_E090_43F0_8D74_9D696AFBE245 */
