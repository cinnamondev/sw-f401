#include "Movement.hpp"
#include "Quadrature.hpp"

Movement::Movement(float outL,float outR,
                   int stepsGoal)
    : pwmOutL(outL), pwmOutR(outR), stepsGoal(stepsGoal) {
}

void Movement::stepsGoalReached() {
  motorL->setPWM(0.5);
  motorR->setPWM(0.5);
  finishCB();
}

void Movement::start(Motor* l, Motor* r, Callback<void()> eventFinish) {
  motorL = l; motorR = r;
  l->setMode(Motor::OperatorControlled);
  l->setPWM(pwmOutL);
  r->setMode(Motor::OperatorControlled);
  r->setPWM(pwmOutR);
  if (finishCB != nullptr) {r->encoder->registerDistanceEvent(stepsGoal,callback(this, &Movement::stepsGoalReached)); }

}

Line::Line(int distance) : Movement(0.7,0.3,distance){

}

Turn::Turn(float degrees) : Movement((degrees > 0) ? 0.7 : 0.3, (degrees > 0) ? 0.7 : 0.3,
                                     degreesToSteps(degrees, 256*8)) {}