#include "Encoder.hpp"

#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_gpio.h"

Encoder::Encoder(TIM_TypeDef* timer, bool startNow) {
  us_ticker_init(); // mbed hal api used for precision timekeeping

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  // stm32 timer initialization
  hwTimer.Instance = timer;
  hwTimer.Init.Period = 0xFFFFFFFF;
  hwTimer.Init.Prescaler = 0;
  hwTimer.Init.ClockDivision = 0;
  hwTimer.Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_TIM_Encoder_MspInit(&hwTimer);

  // configure timer in ENCODER mode. Ensure that the timer is not already running
  // and is valid.
  TIM_Encoder_InitTypeDef encoderCfg;
  encoderCfg.EncoderMode = TIM_ENCODERMODE_TI12; // x4 encoder

  encoderCfg.IC1Polarity = TIM_ICPOLARITY_RISING;
  encoderCfg.IC1Prescaler = TIM_ICPSC_DIV1;
  encoderCfg.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  encoderCfg.IC1Prescaler = TIM_ICPSC_DIV1;
  encoderCfg.IC1Filter = 0;

  encoderCfg.IC2Polarity = TIM_ICPOLARITY_RISING; 
  encoderCfg.IC1Prescaler = TIM_ICPSC_DIV1;
  encoderCfg.IC2Selection = TIM_ICSELECTION_INDIRECTTI;
  encoderCfg.IC2Prescaler = TIM_ICPSC_DIV1;
  encoderCfg.IC2Filter = 0;

  if (HAL_TIM_Encoder_Init(&hwTimer, &encoderCfg) != HAL_OK) {
    printf("Timer is not valid");
    fflush(stdout);
  }

  HAL_TIM_Encoder_Start(&hwTimer, TIM_CHANNEL_ALL);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&hwTimer, &sMasterConfig);
}

void Encoder::update() {
  static uint32_t t1 = 0;
  static uint32_t c1 = 0;
  uint32_t c2 = LL_TIM_GetCounter(hwTimer.Instance);
  uint32_t t2 = us_ticker_read();
  timeDelta = (t2 >= t1) ? t2-t1 : t1-t2; // current time - last time
  t1 = t2;
  stepDelta = (c2>c1) ? c2-c1 : c1-c2; // current counter - last counter
  stepAcc += stepDelta;
  c2 = c1;

  if ((cbDistance != nullptr) && (stepAcc >= pulse_threshold)) {
    cbDistance();
    stepAcc = 0;
    if (!event_repeatable) {
      cbDistance = nullptr;
    }
  }
}
