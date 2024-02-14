#include "Motor.hpp"

Motor::Motor(PinName pwm, PinName current, class Quadrature *_encoder)
    : PID(1000.0,*encoder->stepSpeed,Gains {.kp=1, .ki = 1, .kd = 1}, Range {.min=0,.max=100000}, Range {.min=0,.max=1},10ms,true), pwmOut(pwm), currentMonitor(current), encoder(_encoder){
}

void Motor::setMode(Motor::Controller mode) {
  isPIDRunning = (mode == PID);
}

void Motor::setPWM(float pwm) {
  pwmOut = pwm;
}
void Motor::onCompute(float result) {
  pwmOut = result;
}