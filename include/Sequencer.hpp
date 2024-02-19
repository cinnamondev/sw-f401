#ifndef SW_F401_SEQUENCER_HPP
#define SW_F401_SEQUENCER_HPP
#include "Movement.hpp"
#include <vector>

class Sequencer {
  std::vector<Movement*> movements;
  size_t n = 0;
  Callback<void()> finishedCallback;
  void advance(void);
  Motor* l;
  Motor* r;
public:
  Sequencer(Motor* l, Motor* r);
  Sequencer(Motor* l, Motor* r, std::vector<Movement*> movements);
  void add(Movement* movement);
  void onFinished(Callback<void()> cb) { finishedCallback = cb; }
  void play(void);
};
#endif // SW_F401_SEQUENCER_HPP
