#ifndef SW_F401_SEQUENCER_HPP
#define SW_F401_SEQUENCER_HPP
#include "Movement.hpp"
#include <vector>

class Sequencer {
  DigitalOut* driver_en;
  bool doesRepeat = false;
  std::vector<Movement*> movements;
  size_t n = 0;
  Callback<void()> finishedCallback;
  void advance(void);
  Motor* l;
  Motor* r;
  void startNMovement(void);
public:
  Sequencer(Motor* l, Motor* r, DigitalOut* en);
  Sequencer(Motor* l, Motor* r, DigitalOut* en, std::vector<Movement*> movements);
  void add(Movement* movement);
  void onFinished(Callback<void()> cb) { finishedCallback = cb; }
  void play(void);
  void repeats(bool repeat);
};
#endif // SW_F401_SEQUENCER_HPP
