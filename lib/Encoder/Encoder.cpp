//
// Created by nutmeg on 2/28/24.
//

#include "Encoder.hpp"
Encoder::Encoder(TIM_TypeDef* timer, std::chrono::microseconds pollInterval,
                 bool startNow) {
}

void Encoder::poll() {
  static uint32_t t1 = 0;
  static uint32_t c1 = 0;
  uint32_t c2 = LL_TIM_GetCounter(TIM3);
  uint32_t t2 = us_ticker_read();
  uint32_t t_delta = (t2 >= t1) ? t2-t1 : t1-t2;
  bool c_clockwise;
  uint32_t c_delta;
  if (c2 >= c1) {
    c_clockwise = true;
    c_delta = c2-c1;
  } else {
    c_clockwise = false;
    c_delta = c1-c2;
  }
  c_delta;
  t1 = t2; pulses=c2=c1;
}