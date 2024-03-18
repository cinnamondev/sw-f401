#include "Encoder.hpp"
#include "logging.h"

Encoder::Encoder(TIM_TypeDef* timer, bool startNow) {
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  ststm32_timer->Instance = timer;
  ststm32_timer->Init.Period = 0xFFFFFFFF;
  ststm32_timer->Init.Prescaler = 0;
  ststm32_timer->Init.ClockDivision = 0;
  ststm32_timer->Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_TIM_Encoder_MspInit(ststm32_timer);

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

  if (HAL_TIM_Encoder_Init(ststm32_timer, &encoderCfg) != HAL_OK) {
    DEBUG("Timer is not valid");
  }

  HAL_TIM_Encoder_Start(ststm32_timer, TIM_CHANNEL_ALL);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(ststm32_timer, &sMasterConfig);
}

void Encoder::compute() {
  static uint32_t t1 = 0;
  static uint32_t c1 = 0;
  uint32_t c2 = LL_TIM_GetCounter(ststm32_timer->Instance);
  uint32_t t2 = us_ticker_read();
  uint32_t t_d = (t2 >= t1) ? t2-t1 : t1-t2;
  bool c_clockwise;
  uint32_t c_d;
  if (c2 >= c1) {
    c_clockwise = true;
    c_d= c2-c1;
  } else {
    c_clockwise = false;
    c_d = c1-c2;
  };
  speed = float(c_d) / float(t_d);
  t1 = t2; pulses=c2=c1;
}

Encoder
