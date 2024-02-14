/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @date 2023-10-29
 */

#include "mbed.h"
#include "Motor.hpp"

#include "DS2871.hpp"
/** Blinking rate (ms) */
#define BLINKING_RATE 600ms


/**
 * @brief Blink LED @ rate of BLINKING_RATE.
 */
int main() {
  // Initialise the digital pin LED1 as an output
  DigitalOut led(LED1);

  Quadrature encoder(TIM3);
  Motor motor(PA_0, PA_1, &encoder);
  motor.setPWM(0.5);
  while (true) {
    led = !led;
    ThisThread::sleep_for(BLINKING_RATE);
  }
}
