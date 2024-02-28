#include "Motor.hpp"

Motor::Motor(PinName pwm, PinName current, class QEI *_encoder)
    : encoder(_encoder), PID(Range {.min = 0.0, .max = 33750 }, Range {.min = 0.0, .max = 1.0}), pwmOut(pwm), currentMonitor(current) {
  setMode(OperatorControlled);
  pwmOut.period_us(30);
}

void Motor::setMode(Motor::Controller mode) {
  isPIDRunning = (mode != OperatorControlled);
}

void Motor::setPWM(float pwm) {
  if (isPIDRunning) { return; }
  pwmOut = pwm;
}
void Motor::onCompute(float result) {
  pwmOut = result;
}