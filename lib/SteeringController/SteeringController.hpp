#ifndef STEERINGCONTROLLER_HPP
#define STEERINGCONTROLLER_HPP

#include "PID.hpp"
#include "Motor.hpp"
#include "Movement.hpp"

class SteeringController : protected PID {
public:
  Queue<Movement, 16> movements;
  SteeringController(PID::Config controller, Motor::Config left, Motor::Config right);
private:
  Thread queueWatch;
  Motor left;
  Motor right;
};

#endif // STEERINGCONTROLLER_HPP