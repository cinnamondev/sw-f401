#include "Encoder.hpp"

#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"
#ifndef LIB_ENCODER_NOMBED
Encoder::Encoder(TIM_HandleTypeDef* timer, std::chrono::microseconds sampleRate) {
  us_ticker_init(); // ensure microsecond ticker is enabled
  tickRate = sampleRate;
#else
Encoder::Encoder(TIM_HandleTypeDef* timer) {
#endif
  hwTimer = timer;
}

void Encoder::update() {
#ifndef LIB_ENCODER_NOMBED
  uint32_t t2 = us_ticker_read();
#endif
  uint32_t c2 = __HAL_TIM_GetCounter(hwTimer);
  steps = (c2>=c1) ? c2-c1 : -1 *(c1-c2); // direction sensitive change
#ifndef LIB_ENCODER_NOMBED
  timeDelta = (t2-t1) ? t2-t1 : t1-t2; // prevent weird delta from OF/UF.
  t1 = t2;
#endif
  c1 = c2;
}
#ifndef LIB_ENCODER_NOMBED
void Encoder::start() {
  ticker.attach(callback(this, &Encoder::update), tickRate);
  update();

}

void Encoder::stop() {
  ticker.detach();
}
#endif

int Encoder::getSteps() {
  return steps;
}
