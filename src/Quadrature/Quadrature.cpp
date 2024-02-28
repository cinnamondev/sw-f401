#include "Quadrature.hpp"
#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include <cstdlib>
#include <cstdint>

// TODO: Figure out why Quadrature module is not friendly, in the meantime
// please use QEI.

Quadrature::Quadrature(TIM_TypeDef *timer) {
  // https://github.com/STMicroelectronics/STM32CubeF4/blob/ec8744184587ef76b0e1704d1ee3e391f1aa2b90/Projects/STM324xG_EVAL/Examples/TIM/TIM_Encoder/Src/main.c
  //  configuration code based on ^^
  //  Quadrature encoder configuration

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  encoder.Instance = timer;
  encoder.Init.Period = 0xFFFFFFFF;
  encoder.Init.Prescaler = 0;
  encoder.Init.ClockDivision = 0;
  encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
  HAL_TIM_Encoder_MspInit(&encoder);

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

  if (HAL_TIM_Encoder_Init(&encoder, &encoderCfg) != HAL_OK) {
    printf("Timer used is not valid");
  }

  HAL_TIM_Encoder_Start(&encoder, TIM_CHANNEL_ALL);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

  HAL_TIMEx_MasterConfigSynchronization(&encoder, &sMasterConfig);
}
void Quadrature::tick(void) {
  static unsigned long oldCnt = 0;
  static uint32_t t1 = 0;
  unsigned long cnt = __HAL_TIM_GET_COUNTER(&encoder);
  uint32_t t2 = us_ticker_read(); // us_ticker_read can provide higher res.
  uint32_t delta = (t2 >= t1) ? (t2-t1) : (t1-t2); // prevent UF w/o larger type
      __HAL_TIM_IS_TIM_COUNTING_DOWN(&encoder) ? cnt - oldCnt : oldCnt - cnt;
  stepsTaken += cnt - oldCnt; // considered checking direction or using a larger
                              // size, but this function should not take long!!
  // steps/ms. Converting cnt and oldCnt to float independently lets us
  // do some signed magic. It is important that this is signed!s
  stepSpeed = (float(cnt) - float(oldCnt)) / float(delta);
  oldCnt = cnt; // Current values will be used as old values next 'tick'.
  t1 = t2;


  // Execution of the steps event
  if (stepsTaken >= stepsEventThreshold) {
    stepsTaken = 0;
    if (stepsEvent != nullptr && runStepsEvent) {stepsEvent();}
  }
}

bool Quadrature::getDirection(void) {
  return __HAL_TIM_IS_TIM_COUNTING_DOWN(&encoder);
}

void Quadrature::onStepsReached(Callback<void()> cb, unsigned long steps) {
  stepsEvent = cb;
  stepsEventThreshold = steps;
  runStepsEvent = true;
}

void Quadrature::unregisterStepsEvent() {
  stepsEvent = nullptr;
  runStepsEvent = false;
}


