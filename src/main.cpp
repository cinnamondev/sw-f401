/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief
 * @date 2023-10-29
 */

#include "../c12832/C12832.h"
#include "QEI.hpp"
#include "Sequencer.hpp"
#include "mbed.h"

#include "Bluetooth.hpp"
C12832 display(D11, D13, D12, D7, D10);
// QEI encoderL(PB_6, PB_7, NC, 255);
QEI encoderL(PB_12, PB_7, NC, 255, QEI::X4_ENCODING);
Motor motorL(PB_1, A5, &encoderL);
// Motor* motorL = new Motor(PB_1, A5, encoderL);
// QEI encoderR(PA_6, PA_7, NC, 255);
QEI encoderR(PC_3, PC_2, NC, 255, QEI::X4_ENCODING);
Motor motorR(PC_7, A4, &encoderR);

DigitalOut enable(PB_2, 0);
// Thread displayThread;

int degreesToSteps(float degrees, int stepsPerRevolution) {
  float degs = float(stepsPerRevolution)*(degrees/360.0f);
  return round(degs); // tries to account for fpe
}

int distanceToSteps(float distanceMM, float diameterMM,
                    int stepsPerRevolution) {
  return degreesToSteps((360.0f * distanceMM) / (M_PI * diameterMM),
                        stepsPerRevolution);
}

void refreshDisplay(C12832 *d) {
  while (true) {
    d->cls();
    d->locate(10, 10);
    d->printf("Pulses: %d", encoderR.getPulses());
    ThisThread::sleep_for(10ms);
  }
}

int main() {
  enable = 0;
  motorL.setPWM(0.5);
  motorR.setPWM(0.5);
  // displayThread.start(callback(&refreshDisplay, &display));
  int S1M = distanceToSteps(50, 80, 256*8);
  Sequencer sequencer(&motorL, &motorR, &enable,
                      std::vector<Movement *>{
                          new Line(S1M),
                          new Turn(90),
                          new Line(S1M),
                          new Turn(90),
                          new Line(S1M),
                          new Turn(90),
                          new Line(S1M),
                          new Turn(180),
                          new Line(S1M),
                          new Turn(-90),
                          new Line(S1M),
                          new Turn(-90),
                          new Line(S1M),
                          new Turn(-90),
                          new Line(S1M),
                      });
  sequencer.repeats(false);
  sequencer.play();

  while (true)
    ;
}