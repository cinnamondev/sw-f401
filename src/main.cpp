/**
 * @file main.cpp
 * @author cinnamondev
 * @date 2023-10-29
 *
 * Technical Demo A program Task 3/4
 */

#include "mbed.h"

//PwmOut motorL(PA_8);
PwmOut motorL(PB_1);
PwmOut motorR(PC_7);

InterruptIn enableSwitch(A2);
InterruptIn disableSwitch(A3);
int main() {
  auto *enable = new DigitalOut(PB_2, 0);
  AnalogIn potL(A0);
  AnalogIn potR(A1);

  motorL.period_us(20);
  motorR.period_us(20);
  motorL=0.5;
  motorR = 0.5;
  // QEI encoderR(PB_13, PB_14, NC, 255);
  enableSwitch.fall(callback([enable]() { enable->write(0); }));
  disableSwitch.fall(callback([enable]() { enable->write(1); }));
  float outL = 0.5;
  float outR = 0.5;
  while (true) {
    outL = potL.read();
    outR = potR.read();
    if ((0.45 < outL) & (outL < 0.55)) {
      outL = 0.5;
    }
    if ((0.45 < outR) & (outR < 0.55)) {
      outR = 0.5;
    }
     motorL = outL;
    motorR = outR;
  }
}
