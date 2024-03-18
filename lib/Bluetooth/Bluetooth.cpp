

#include "Bluetooth.hpp"

Bluetooth::Bluetooth(mbed::BufferedSerial *serial,
                     std::vector<Command> commands, bool startNow)
    : s(serial), commands(std::move(commands)) {
  s->set_blocking(false);
  if (startNow) {
    start();
  }
}

void Bluetooth::onSigio() {
  sQueue->call(this, &Bluetooth::poll); // Defer to shared queue context
}
void Bluetooth::poll() {
  uint8_t cIn; 
  if (s->read(&cIn, 1) > 0) { // store latest command & parse.
    commandParser(cIn);
  }
  s->sync();  // flush anything left in the buffer
}

void Bluetooth::commandParser(uint8_t cmd) {
  for (const auto &c : commands) { // linear search for matching command
    if ((cmd & c.mask) == c.cmd) { // only match operand
      c.action(cmd);
      break;
    }
  }
}

void Bluetooth::start() {
  s->sigio(callback(this, &Bluetooth::onSigio));
}

void Bluetooth::stop() { s->sigio(nullptr); }

// Command control

void Bluetooth::addCommand(Bluetooth::Command cmd) { commands.push_back(cmd); }

void Bluetooth::removeCommand(Bluetooth::Command *cmd) {
  removeCommand(cmd->cmd);
}

void Bluetooth::removeCommand(uint8_t cmd) {
  // this is a bit expensive so we should avoid
  // removing commands, but its here.
  commands.erase(std::remove_if(commands.begin(), commands.end(),
                                [cmd](const auto c) { return c.cmd == cmd; }),
                 commands.end());
}

Bluetooth::Command::Command(uint8_t cmd, uint8_t mask,
                            Callback<void(uint8_t)> action)
    : cmd(cmd & mask), mask(mask), action(action) {}