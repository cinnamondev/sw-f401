/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @date 2023-10-29
 */

#include "mbed.h"
#include "Sequencer.hpp"
#include "QEI.hpp"
#include "../c12832/C12832.h"
#include "stm32f4xx.h"

EventQueue q;

int main() {
  C12832 display(NC,NC,NC,NC,NC);
  QEI encoderR(PB_13, PB_14, NC, 255);
  encoderR.reset();
  Motor motorR(D9, D8, &encoderR);
  Sequencer sequencer(&motorR,&motorR,std::vector<Movement*> {new Line(1000), new Turn(90), new Line(1000), new Turn(90), new Line(1000), new Turn(180)});
  sequencer.repeats(true);
  sequencer.play();
  while(true) {
    display.cls();
    display.locate(10,10);
    display.printf("Pulses: %d", encoderR.getPulses());
  }
}
