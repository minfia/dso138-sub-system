/*
 * @file    port.h
 * @brief   IOポートドライバヘッダ
 *
 * @author  minfia
 */ 


#ifndef INCLUDE_GUARD_UUID_944EA987_8C0E_4841_A169_A4A79038AE3E
#define INCLUDE_GUARD_UUID_944EA987_8C0E_4841_A169_A4A79038AE3E


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * @brief   IOポートドライバ初期化
 */
void port_init();

/**
 * @brief   ポートをOutputに設定
 *
 * @param [in]  port    ポート名(PORTA, PORTB,...)
 * @param [in]  num     ポート番号(0,1,..)
 */
void port_set(volatile uint8_t *port, uint8_t num);

/**
 * @brief   ポートをInputに設定
 *
 * @param [in]  port    ポート名(PORTA, PORTB,...)
 * @param [in]  num     ポート番号(0,1,..)
 */
void port_clear(volatile uint8_t *port, uint8_t num);

/**
 * @brief   ポートの状態を取得
 *
 * @param [in]  port    ポート名(PORTA, PORTB,...)
 * @param [in]  num     ポート番号(0,1,..)
 *
 * @retval  0:Input, 1:Output
 */
uint8_t port_get(volatile uint8_t *port, uint8_t num);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* INCLUDE_GUARD_UUID_944EA987_8C0E_4841_A169_A4A79038AE3E */
