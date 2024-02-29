/**
 * @file Bluetooth.hpp
 * @author cinnamondev
 *
 * HM-10 Bluetooth Module
 */

#ifndef SW_F401_HM10_HPP
#define SW_F401_HM10_HPP

#include "mbed.h"
#include <vector>

/** Default ticker period for polling for bluetooth commands */
#define BLUETOOTH_DEFAULT_POLLING_PERIOD 100ms
/**
 * Bluetooth Module for the HM-10 that polls for bluetooth inputs ('commands'),
 * which can have code executed upon being received (polling is achieved via
 * BufferedSerial usage).
 */
class Bluetooth {
public:
  /** A single byte bluetooth command. */
  struct Command {
    /** Byte corresponding to command. It is reccomended that bits intending to
     * contain parameters are zero'd out, though as long as the mask is correct,
     * this will not be an issue.
     */
    uint8_t cmd;
    /** Mask should cover all bits of the command that are NOT opcodes. This is
     * used to parse commands that contain any parameters. */
    uint8_t mask;
    /**
     * Callback to execute when input & mask = command. Passes the un-masked
     * command, and the serial interface used, so that the user may implement
     * a response.
     *
     * All callbacks are executed in shared queue context, so while blocking
     * calls are available, it is still reccomended to limit your execution
     * time.
     *
     * @note Response via BufferedSerial requires all the same checks (is
     * writable).
     */
    Callback<void(uint8_t, BufferedSerial*)> action;
    /**
     * Construct an instance of a Bluetooth Command
     * @param cmd Unique byte corresponding to this command (use mask to define
     * opcode bitfields)
     * @param mask Masks the opcode segment of the command.
     * @param action Callback to call when command is receieved
     */
    Command(uint8_t cmd, uint8_t mask, Callback<void(uint8_t, BufferedSerial*)> action);
  };
  /**
   * Creates a Bluetooth object.
   * @param serial BufferedSerial instance to use. For regular operation,
   * avoid reading from this instance outside of Bluetooth, though it should
   * be accessible in-case you want/need to redirect standard output to it.
   * @param commands Initial commands to register immediately.
   * @param startNow Pass true if you want to start polling for commands
   * immediately.
   * @param pollInterval Configure this if you are starting immediately AND
   * want to change how often new commands are checked for. (If started later,
   * this value means nothing).
   *
   * @note The BufferedSerial instance will be configured on initialization to
   * be 'non-blocking' (does not wait for data).
   */
  explicit Bluetooth(BufferedSerial* serial,
            std::vector<Command> commands = {},
            bool startNow = false,
            std::chrono::microseconds pollInterval = BLUETOOTH_DEFAULT_POLLING_PERIOD);
  void addCommand(Bluetooth::Command cmd);
  /**
   * Removes a command.
   * @note Calling this function is pretty expensive, it is not recommended
   * that your implementation is constantly adding and removing commands.
   * @param cmd Bluetooth command to remove
   */
  void removeCommand(Bluetooth::Command* cmd);
  void removeCommand(uint8_t cmd);
  /**
   * Starts/resumes polling for bluetooth commands.
   * @param pollInterval How often to poll for bluetooth commands.
   */
  void start(std::chrono::microseconds pollInterval = BLUETOOTH_DEFAULT_POLLING_PERIOD);
  /** Stops polling for bluetooth commands, if not already done so */
  void stop();
private:
  /** Shared event queue to defer longer execution to. */
  EventQueue* sQueue = mbed_event_queue();
  Ticker ticker;
  BufferedSerial* s;
  /** Collection of registered commands */
  std::vector<Command> commands;
  /**
   * Periodically pulls a byte from the buffer to see if new commands have been
   * received. If a new command is received, it will make a deferred call to
   * `commandParser` (in shared queue context).
   */
  void poll();
  /**
   * Parses a command and executes the callback, if it is added to Bluetooth.
   * @note If additional implementations call this function, avoid calling from
   * ISR context. Default/Current implementation is to call this on the shared
   * event thread. Reason being is that it's lifetime corresponds to N commands
   * present in `commands`, and that actions may want to call blocking behaviour,
   * which might panic in ISR context, or just take a while.
   * @param cmd Command byte to look for (ignore operands/fill operands as 0's)
   */
  void commandParser(uint8_t cmd);
};

#ifdef CONFIG_REDIRECT_STDIO_BLE

#endif
#endif // SW_F401_HM10_HPP
