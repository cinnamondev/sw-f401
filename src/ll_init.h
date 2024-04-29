#ifndef SW_F401_LLINITCODE_HPP
#define SW_F401_LLINITCODE_HPP

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @file ll_init.c
 * @brief STM32
 */

/* Includes ------------------------------------------------------------------*/
#include "ll_init.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_uart.h"

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
 */
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);
void MX_USART2_UART_Init(void);
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_encoder);
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_encoder);
void HAL_UART_MspInit(UART_HandleTypeDef* huart);
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart);
#ifdef __cplusplus
}
#endif
#endif // SW_F401_LLINITCODE_HPP
