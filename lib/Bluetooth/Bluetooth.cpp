#include "Bluetooth.hpp"

Bluetooth::Bluetooth(mbed::BufferedSerial *serial,
                     std::vector<Command> commands, bool startNow,
                     std::chrono::microseconds pollInterval)
    : s(serial), commands(std::move(commands)) {
  if (startNow) {
    start();
  }
}

void Bluetooth::onSigio() {
  sQueue->call(this, &Bluetooth::poll);
}
void Bluetooth::poll() {
  uint8_t cIn; // Store latest command
  if (s->read(&cIn, 1) > 0) { // flush + process
    commandParser(cIn);
  }
  s->sync();
}

void Bluetooth::addCommand(Bluetooth::Command cmd) { commands.push_back(cmd); }

void Bluetooth::removeCommand(Bluetooth::Command *cmd) {
  removeCommand(cmd->cmd);
}

void Bluetooth::commandParser(uint8_t cmd) {
  for (const auto &c : commands) {
    if ((cmd & c.mask) == c.cmd) {
      c.action(cmd);
      break;
    }
  }
}
void Bluetooth::removeCommand(uint8_t cmd) {
  commands.erase(std::remove_if(commands.begin(), commands.end(),
                                [cmd](const auto c) { return c.cmd == cmd; }),
                 commands.end());
}

void Bluetooth::start() {
  s->sigio(callback(this, &Bluetooth::onSigio));
}

void Bluetooth::stop() { s->sigio(nullptr); }

Bluetooth::Command::Command(uint8_t cmd, uint8_t mask,
                            Callback<void(uint8_t)> action)
    : cmd(cmd & mask), mask(mask), action(action) {}