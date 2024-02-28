/**
 * @file main.cpp
 * @author cinnamondev
 * @date 2023-10-29
 *
 * Technical Demo A program Task 3/4
 */

#include "mbed.h"
#include "QEI.hpp"
#include "C12832.h"

DigitalOut ledPWML(D8);
DigitalOut ledPWMR(D5);

//PwmOut motorL(PA_8);
PwmOut motorL(PB_1);
PwmOut motorR(PC_7);

InterruptIn enableSwitch(A2);
InterruptIn disableSwitch(A3);
int main() {
  C12832 d(D11, D13, D12, D7, D10);
  d.cls();
  auto *enable = new DigitalOut(PB_2, 0);
  AnalogIn potL(A0);
  AnalogIn potR(A1);
  QEI encoderL(PB_12, PB_7, NC, 255, QEI::X4_ENCODING);
  QEI encoderR(PC_3, PC_2, NC, 255, QEI::X4_ENCODING);
  int pl1 = encoderL.getPulses();
  int pr1 = encoderR.getPulses();
  motorL.period_us(30);
  motorR.period_us(30);
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
  int pl2 = encoderL.getPulses();
  int pr2 = encoderR.getPulses();
  if (abs(pl2-pl1) > 10) {
    ledPWML.write(1);
  } else {
    ledPWML.write(0);
  }
  if (abs(pr2-pr1) > 10) {
    ledPWMR.write(1);
  } else {
    ledPWMR.write(0);
  }
}
