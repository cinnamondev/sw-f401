/**
 * Motor PID CMSIS-DSP
 */
#ifndef PID_HPP
#define PID_HPP

#include "mbed.h"
#include "arm_math.h"
#include <limits>

#ifndef PID_CONTROL_INTERVAL
#define PID_CONTROL_INTERVAL 1ms
#endif

/**
 * @brief CMSIS-DSP PID wrapper 
 * 
 * Use via polymorphism and override virtual methods to implement neccesary logic
 */
class PID {
public:
  struct Config {
    float kp;
    float ki;
    float kd;
    float in_min; 
    float in_max;
    float out_min; 
    float out_max;
    float sp;
    float* pv;
  };
  PID(PID::Config, std::chrono::microseconds tickRate, bool startNow = false);
  ~PID();
  void start(void) { running = true; }
  void stop(void) { running = false;}
  bool isRunning(void) { return running; }
  void reset(void) { arm_pid_reset_f32(&pid); }
  void setGains(float kp, float ki, float kd);
  PID::Config getConfig(void) {
    return config;
  }
  virtual void setConfig(PID::Config);
  void inputRange(float min, float max);
  void setProcess(float* pv) {
    config.pv = pv;
  }
  void outputRange(float min, float max);
  void setTarget(float sp) {
    config.sp = sp;
  }
protected:
  virtual void onCompute(float) {}; /**< Override to pass PID output to your implementation*/
  virtual void preCompute(void) {}; /**< Override to have logic execute before computation */
  arm_pid_instance_f32 pid;
  PID::Config config;
private:
  Ticker ticker;
  bool running = false;
  void tick(void);
};


#endif //PID_HPP
