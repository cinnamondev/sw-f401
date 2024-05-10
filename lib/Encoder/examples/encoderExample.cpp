#include "Encoder.hpp"
#include "mbed.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_tim.h"

TIM_HandleTypeDef htim4;

// STM32CUBEMX Initialization Functions (See MX_TIM4_INIT & MspInit) 

void MX_TIM4_Init(void);
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim_encoder);
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef *htim_encoder);
void Error_Handler();

Encoder encoder(&htim4, 1ms);

int main(void) {
  encoderLeft.start();
  while (true) {
    printf("%d", encoder.getSteps()); // every ms encoder will update and
                                          // this value will be changed.
                                          // if you want to do this manually,
                                          // dont start the encoder and call
                                          // Encoder::update() manually.
  }
}

/**
 * @brief TIM4 Initialization Function
 * @param None
 * @retval None
 */
void MX_TIM4_Init(void) {

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 0;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 65535;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 0;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 0;
  if (HAL_TIM_Encoder_Init(&htim4, &sConfig) != HAL_OK) {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */

  /* USER CODE END TIM4_Init 2 */
}

/**
 * @brief TIM_Encoder MSP Initialization
 * This function configures the hardware resources used in this example
 * @param htim_encoder: TIM_Encoder handle pointer
 * @retval None
 */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim_encoder) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (htim_encoder->Instance == TIM4) {
    /* USER CODE BEGIN TIM4_MspInit 0 */

    /* USER CODE END TIM4_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM4 GPIO Configuration
    PB6     ------> TIM4_CH1
    PB7     ------> TIM4_CH2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USER CODE BEGIN TIM4_MspInit 1 */

    /* USER CODE END TIM4_MspInit 1 */
  }
}

/**
 * @brief TIM_Encoder MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param htim_encoder: TIM_Encoder handle pointer
 * @retval None
 */
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef *htim_encoder) {
  if (htim_encoder->Instance == TIM4) {
    /* USER CODE BEGIN TIM4_MspDeInit 0 */

    /* USER CODE END TIM4_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();

    /**TIM4 GPIO Configuration
    PB6     ------> TIM4_CH1
    PB7     ------> TIM4_CH2
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);

    /* USER CODE BEGIN TIM4_MspDeInit 1 */

    /* USER CODE END TIM4_MspDeInit 1 */
  }
}

void Error_Handler(void) {
    while(1) { __NOP(); } // spin lock 
}