#ifndef PERSISTENTMEMORY_HPP
#define PERSISTENTMEMORY_HPP

// https://tronche.com/blog/2020/03/mbed-flashiap-tdbstore-and-stm32f4-internal-flash/
// TODO: Currently dummy class. see above for future implementation plans.

struct PID {
  float kp,ki,kd;
};

// TODO: api is bound to change again this is just a there abouts representation
void* getData(const char key) {
  PID pidPID;
  pidPID.kp = 1.0;
  pidPID.ki = 1.0;
  pidPID.kd = 1.0;

  PID pidUSR;
  pidUSR.kp = 1.0;
  pidUSR.ki = 1.0;
  pidUSR.kd = 1.0;
  switch (key) {
  case 'p':
    return (void*) &pidPID;
    break;
  case 'u'`i1:
    return (void*) &pidUSR;
    break;
  default:
    break;
  }
}
#endif // PERSISTENTMEMORY_HPP
