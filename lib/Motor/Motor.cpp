#include "Motor.hpp"

Motor::Motor(Motor::Config config, TIM_HandleTypeDef* tim) : PID(config.pid, 1ms, false), pwmOut(config.pwm), speed(0), encoder(tim,1ms){
  setProcess(&speed);
  pwmOut.period_us(30);
  pwmOut = 0.5;
  encoder.start();
}

void Motor::onCompute(float v) {
  pwmOut = v;
}

void Motor::preCompute() {
  speed = encoder.getSpeed();
}


void Motor::setSpeed(float stepsPTick) {
  config.sp = stepsPTick;
  arm_pid_reset_f32(&pid);
}
void Motor::setConfig(PID::Config _config) {
  setGains(_config.kp, _config.ki, _config.kd);
}
