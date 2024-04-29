#ifndef SW_F401_ENCODER_HPP
#define SW_F401_ENCODER_HPP

#include "mbed.h"
#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"

class Encoder {
public:
  explicit Encoder(TIM_TypeDef* timer);
  float getSpeed() const { return float(stepDelta)/float(timeDelta*1e6); }
  unsigned int getSteps() { return stepAcc; stepAcc = 0; }
  void update();
  void onDistance(int distance, Callback<void()> e, bool repeat = false);
  static float degreesToSteps(int steps, unsigned int cpr);
  static float distanceToSteps(float distance, float wheelDiameter, unsigned int cpr);
private:
  TIM_HandleTypeDef* hwTimer;
  volatile uint32_t stepAcc = 0;
  volatile unsigned int stepDelta = 0;
  volatile uint64_t timeDelta = 0;
  unsigned int pulse_threshold = 0;
  bool event_repeatable = false;
  Callback<void()> cbDistance;
};


#endif // SW_F401_ENCODER_HPP
