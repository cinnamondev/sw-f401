#ifndef SW_F401_MOVEMENT_HPP
#define SW_F401_MOVEMENT_HPP
#include "Motor.hpp"

class Movement {
  float pwmOutL;
  float pwmOutR;
  int stepsGoal;
  Callback<void()> finishCB;
  Motor* motorL;
  Motor* motorR;
public:
  Movement(float outL, float outR, int stepsGoal);
  void onStepsGoal(Callback<void()> onGoal) { finishCB = onGoal; }
  void stepsGoalReached(void);
  void start(Motor* l, Motor* r, Callback<void()> eventFinish);
};

class Line : public Movement {
public:
  Line(int distance);
};

class Turn : public Movement {
public:
  Turn(float degrees);
};

#endif // SW_F401_MOVEMENT_HPP
