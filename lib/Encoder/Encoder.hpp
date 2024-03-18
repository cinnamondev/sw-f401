//
// Created by nutmeg on 2/28/24.
//

#ifndef SW_F401_ENCODER_HPP
#define SW_F401_ENCODER_HPP
#include "mbed.h"

#include "stm32f4xx.h"
#include "stm32f4xx_ll_tim.h"

class Encoder {
public:
  Encoder(TIM_TypeDef* timer, bool startNow = false);
  int getSpeed() { return speed; }
  unsigned int getPulses() { return pulses; }
  void compute();
  void onDistance(int distance, Callback<void()> e, bool repeat = false);
  static float degreesToSteps(int steps, unsigned int cpr);
  static float distanceToSteps(float distance, float wheelDiameter, unsigned int cpr);
private:
  TIM_HandleTypeDef* ststm32_timer;
  volatile unsigned int pulses = 0;
  volatile float speed = 0;
  volatile uint64_t tick_us = 0;
  Callback<void()> cbDistance;
};


#endif // SW_F401_ENCODER_HPP
