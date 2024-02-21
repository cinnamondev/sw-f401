#ifndef BLUETOOTH_HPP
#define BLUETOOTH_HPP

#include <vector>
#include "mbed.h"

/**
 * Provides an interface to execute code based on specific byte's being
 * received to the HM-10 bluetooth module.
 */
class Bluetooth {
public:
  struct Command {
    /** Unique Opcode/Command corresponding to the command.
     * Bits not masked by the `mask` will be ignored.
     * \example For desired command 0b101100XX (Where X is operand data)
     * .cmd = 0b10110000 (final 2 bits here do not matter)
     * .mask= 0b11111100
     * */
    char cmd = 0x00;
    /**
     * Mask that corresponds to bits relevant to the opcode/command. Defaults to
     * 0xFF ("no operands")
     */
    char mask = 0xFF;
    Callback<void(char)> executeCommand = nullptr;
    /**
     * Returns a fully initialized Bluetooth::Command
     * @param cmd Unique command to detect / opcode
     * @param callback Callback to execute when command is recieved (data passed
     * will be the un-masked command)
     * @param mask Opcode / command mask.
     * @return Fully initialized command
     */
    static Command build(char cmd, Callback<void(char)> callback, char mask = 0xFF);
  };
  /**
   * Create a HM10 Bluetooth Module
   * @param tx UART TX Pin
   * @param rx UART RX Pin
   * @param cmds Default command set (optional)
   */
  Bluetooth(PinName tx, PinName rx, std::vector<Command> cmds = {});
  /**
   * Start polling for new commands
   * @param pollingInterval Polling period (defaults to 100 ms)
   */
  void start(std::chrono::microseconds pollingInterval = 100ms);
  /**
   * Stop polling for new commands
   */
  void stop(void);
  /**
   * Add a new command to check for
   * @param cmd Fully populated command
   */
  void addCommand(Command cmd);
  /**
   * Registers a new Bluetooth::Command
   * @param cmd 'Opcode' of the command
   * @param callback Callback to execute when the command has been received
   * @param mask Opcode mask.
   * @note The bluetooth module expects that `cmd` is a unique identifier.
   * The first found instance of a matching `cmd` will be the only one that is
   * executed.
   */
  void addCommand(char cmd, Callback<void(char)> callback, char mask = 0xFF);
  /**
   * Removes all instances of a Bluetooth::Command with matching `cmd`/opcode
   * @param cmd Matching command identifier/opcode
   */
  void removeCommand(char cmd);
  /**
   * Removes all bluetooth commands.
   */
  void reset(void);
  bool isStarted(void) { return started;};
private:
  bool started = false;
  Ticker ticker;
  BufferedSerial hm10;
  char b = 0;
  /** List of currently registered bluetooth commands */
  std::vector<Command> commands;
  /**
   * Checks if the bluetooth module has received new inputs, and executes
   * the first command that it matches. Executes on a regular interval (ticker
   * function)
   */
  void tick(void);
};

#endif