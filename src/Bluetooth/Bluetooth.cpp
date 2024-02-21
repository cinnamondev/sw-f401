#include "Bluetooth.hpp"

#include <utility>

Bluetooth::Bluetooth(PinName tx, PinName rx, std::vector<Command> cmds)
    : hm10(new BufferedSerial(tx, rx, 9600)), commands(std::move(cmds)) {
  hm10->set_blocking(false);
}

Bluetooth::Bluetooth(BufferedSerial *module, vector<Command> cmds) : hm10(module), commands(cmds) {
  hm10->set_blocking(false);
}

void Bluetooth::tick(void) {
  if (hm10->read(&b, 1) == EAGAIN) { return; }
  for (const auto customCommand : commands) {
    if ((b & customCommand.mask) == customCommand.cmd) {
      if (customCommand.executeCommand != nullptr) {
        customCommand.executeCommand(b);
      }
      break;
    }
  }
}
void Bluetooth::start(std::chrono::microseconds interval) {
  started = true;
  ticker.attach(callback(this, &Bluetooth::tick), interval);
}

void Bluetooth::stop(void) {
  started = false;
  ticker.detach();
}

void Bluetooth::addCommand(Bluetooth::Command command) {
  commands.push_back(command);
}
void Bluetooth::addCommand(char cmd, Callback<void(char)> callback, char mask) {
  commands.push_back(Bluetooth::Command::build(cmd, callback, mask));
}
void Bluetooth::removeCommand(char cmd) {
  commands.erase(std::remove_if(
              commands.begin(), commands.end(),
              [cmd](const auto customCommand) {
                return customCommand.cmd == cmd;
              }), commands.end());
}

void Bluetooth::reset(void) {
  commands = {};
}

Bluetooth::Command Bluetooth::Command::build(char cmd, Callback<void(char)> callback, char mask) {
  return Bluetooth::Command {
      .cmd = cmd,
      .mask = mask,
      .executeCommand = callback,
  };
}
