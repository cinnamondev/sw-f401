#include "PID.hpp"

PID::PID(float setPoint, float *process, PID::Gains gains, PID::Range inputRange, PID::Range outputRange, std::chrono::microseconds computeInterval, bool startImmediately = false) {
    setSetPoint(setPoint);
    setPV(process);
    setGains(gains);
    setInputRange(inputRange);
    setOutputRange(outputRange);
    setInterval(computeInterval);
    if (startImmediately) { start(); }
}
PID::PID(PID::Gains gains, PID::Range inputRange, PID::Range outputRange, std::chrono::microseconds computeInterval) {
  setGains(gains);
  setInputRange(inputRange);
  setOutputRange(outputRange);
}

void PID::start(void) {
  arm_pid_reset_f32(&pid);
  running = true;
  ticker.attach(callback(this,&PID::tick), interval);
}
void PID::stop(void) {
  ticker.detach();
  running = false;
}
void PID::reset(void) {
  arm_pid_reset_f32(&pid);
}
bool PID::isRunning(void) { return running; }
void PID::setGains(float kp, float ki, float kd) {
  pid.Kp = kp; pid.Ki = ki; pid.Kd = kd;
  arm_pid_init_f32(&pid,0);
}

void PID::setGains(Gains g) {
  pid.Kp = g.kp;
  pid.Ki = g.ki;
  pid.Kp = g.kp;
}
void PID::setInterval(std::chrono::microseconds _interval) {
  interval = _interval;
  // TODO: PID SCALING INSERT HERE.
  if(running) { // Restart
    stop();
    start();
  }
}

std::chrono::microseconds PID::getInterval(void) {
  return interval;
}
void PID::setSetPoint(float sp) {
  setPoint = sp;
}
void PID::setPV(float *pv) {
  process = pv;
}
void PID::setInputRange(float min, float max) {
  inRange.min = min; inRange.max = max; inRange.range = max-min;
}

void PID::setInputRange(PID::Range r) {
  inRange = r; inRange.range = r.max - r.min;
}

void PID::setOutputRange(float min, float max) {
  outRange.min = min; outRange.max = max; outRange.range = max-min;
}
void PID::setOutputRange(PID::Range r) {
  outRange = r; outRange.range = r.max - r.min;
}

float PID::getSetPoint(void) {
  return setPoint;
}

float PID::getProcessVariable(void) {
  return *process;
}

PID::Gains PID::getGains(void) {
  return Gains {pid.Kp, pid.Ki, pid.Kd};
}


PID::Range PID::getInputRange(void) {
  return inRange;
}

PID::Range PID::getOutputRange(void) {
  return outRange;
}

float PID::compute(void) {
  // Input/Process and set point are now a unit scale to work with
  processLast = *process;
  float scaledInput = (*process - inRange.min) / (inRange.max - inRange.min);
  float scaledSetPoint = (setPoint - inRange.min) / (inRange.max - inRange.min);
  float output = arm_pid_f32(&pid, scaledInput - scaledSetPoint);

  // CMSIS-DSP is poorly documented. state[2] appears to be the internal output.
  if (output > outRange.max) { // Clamp output
    output = outRange.max;
  } else if (output < outRange.min) {
    output = outRange.min;
  }
  pid.state[2] = output;
}

void PID::tick(void) {
    onCompute(compute());
}