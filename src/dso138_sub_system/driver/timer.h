/**
 * @file timer.h
 * @brief TIMERドライバ
 *
 * @author minfia
 */ 


#ifndef INCLUDE_GUARD_UUID_710B2A29_4EF5_418F_B6D5_5CA507FA379E
#define INCLUDE_GUARD_UUID_710B2A29_4EF5_418F_B6D5_5CA507FA379E


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @brief   タイマー0の初期化
 */
void timer0_init(void);

/**
 * @brief   タイマー0カウントスタート
 */
void timer0_start(void);

/**
 * @brief   タイマー0カウントストップ
 */
void timer0_stop(void);

/**
 * @brief   タイマー0カウント再開
 */
void timer0_resume(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* INCLUDE_GUARD_UUID_710B2A29_4EF5_418F_B6D5_5CA507FA379E */
