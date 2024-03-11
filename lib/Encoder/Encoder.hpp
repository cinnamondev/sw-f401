//
// Created by nutmeg on 2/28/24.
//

#ifndef SW_F401_ENCODER_HPP
#define SW_F401_ENCODER_HPP
#include "mbed.h"

#include "stm32f4xx.h"
#include "stm32f4xx_ll_tim.h"

#ifndef ENCODER_DEFAULT_POLL_PERIOD
#define ENCODER_DEFAULT_POLL_PERIOD 10ms
#endif

class Encoder {
public:
  Encoder(TIM_TypeDef* timer, std::chrono::microseconds pollInterval = ENCODER_DEFAULT_POLL_PERIOD, bool startNow = false);
  int getSpeed() { return speed; }
  unsigned int getPulses() { return pulses; }
  void distanceEvent(int distance, Callback<void()> e, bool repeat = false);
  static float degreesToSteps(int steps, unsigned int cpr);
  static float distanceToSteps(float distance, float wheelDiameter, unsigned int cpr);

  void start(std::chrono::microseconds pollInterval = ENCODER_DEFAULT_POLL_PERIOD);
  void stop();
private:
  Ticker ticker;
  TIM_TypeDef* encoder;
  volatile unsigned int pulses = 0;
  volatile int speed = 0;
  volatile uint64_t tick_us = 0;
  Callback<void()> onDistance;

  void poll();
};


#endif // SW_F401_ENCODER_HPP
