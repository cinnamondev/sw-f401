#include "PID.hpp"

void PID::setConfig(PID::Config config) {
    inMin = config.in_min;
    inMax = config.in_max;
    outMin = config.out_min;
    outMax = config.out_max;
    setPoint = config.sp;
    process = config.pv;
    setGains(config.kp, config.ki, config.kd);
}

void PID::setGains(float kp, float ki, float kd) {
  pid.Kp = kp; pid.Ki = ki; pid.Kd = kd;
  arm_pid_init_f32(&pid, !running);
}

void PID::tick() {
  preCompute();
  float res = compute();
  onCompute(res);
}

float PID::compute() {
  us_ticker_init();
    arm_pid_f32(&pid, *process);
    return 0.0f;
}