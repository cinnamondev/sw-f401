#ifndef __MOTOR_INC_
#define __MOTOR_INC_

#include "mbed.h"
#include "Quadrature.hpp"

// compile time mode define MOTOR_USE_BIPOLAR alt mode
class Motor {
    PwmOut motor;
    AnalogIn current;
    Quadrature encoder;

    public:
    enum Direction {
        Forwards,
        Backwards
    }
    enum ControlMode {
        Quadrature,
        Current
    }

};

#endif