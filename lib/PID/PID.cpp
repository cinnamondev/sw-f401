#include "PID.hpp"

PID::PID(PID::Config config, std::chrono::microseconds tickRate, bool startNow) {
  setConfig(config);
  running = startNow;
  ticker.attach(callback(this, &PID::tick), tickRate);
}

PID::~PID() {
  ticker.detach();  
}
void PID::setConfig(PID::Config _config) {
  config = _config;
  setGains(config.kp, config.ki, config.kd);
}

void PID::setGains(float kp, float ki, float kd) {
  pid.Kp = kp; pid.Ki = ki; pid.Kd = kd;
  arm_pid_init_f32(&pid, !running);
}

void PID::tick() {
  preCompute();
  if (!running) { return; }
  float input;
  if (config.in_min > *config.pv) { input = config.in_min; }
  else if (*config.pv < config.in_max) { input = config.in_max; }
  else { input = *config.pv; }
  float error = input - config.sp;
  float res = arm_pid_f32(&pid, error);
  if (res < config.in_min) {
    res = (pid.state[2] = config.in_min);
  } else if (res > config.in_max) {
    res = (pid.state[2] = config.in_max);
  }
  onCompute(res);
}
