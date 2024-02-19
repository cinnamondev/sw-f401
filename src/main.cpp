/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @date 2023-10-29
 */

#include "mbed.h"
#include "Sequencer.hpp"
#include "stm32f4xx.h"
/** Blinking rate (ms) */
#define BLINKING_RATE 600ms

void portInit(void) {
  __HAL_RCC_GPIOB_CLK_ENABLE();
  /*Configure GPIO pin : PC4 */
  GPIO_InitTypeDef gpiob67;
  gpiob67.Pin = GPIO_PIN_6 | GPIO_PIN_7;
  gpiob67.Alternate = GPIO_AF2_TIM4;
  gpiob67.Mode = GPIO_MODE_INPUT;
  gpiob67.Pull = GPIO_PULLUP;

  HAL_GPIO_Init(GPIOB, &gpiob67);
}
int main() {
  //__HAL_RCC_TIM3_CLK_ENABLE();
  //portInit();
  //Ticker quadratureUpdate;

  //  Quadrature encoderL(TIM3);
  //  Quadrature encoderR(TIM4);
  //quadratureUpdate.attach(callback([&encoderR]() {
  //                            encoderR.tick();
  //                          }), 10ms);
  QEI encoderR(D0, D1, NC, 255);
  // d5 d9 quadrature tim3
  //Motor motorL(D1, D2, &encoderL);
  Motor motorR(A3, A4, &encoderR);
  Sequencer sequencer(&motorR,&motorR,std::vector<Movement*> {new Line(1000), new Line(1000)});
  sequencer.play();
  while (true);
}
