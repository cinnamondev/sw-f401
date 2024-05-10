/**
 * @file main.cpp
 * @brief Line following buggy firmware
 * Line following buggy
 */

#include "main.h"
#include "Bluetooth.hpp"
#include "BuggyControl.hpp"
#include "FlashIAPBlockDevice.h"
#include "LineSensor.hpp"
#include "Motor.hpp"
#include "TDBStore.h"
#include "defaultConfigs.hpp"
#include "ll_init.h"
#include "logging.h"

/**
 * @brief Macro Retrieve PID Configuration from TDBStore database
 * @param key Matching key to retrieve from (string literal)
 */
#define TRY_GET_PID(key, configPtr, fallback)                                  \
  {                                                                            \
                                                                               \
    if (int macroerr = tdbStore.get(key, configPtr, sizeof(PID::Config));      \
        macroerr != MBED_SUCCESS) {                                            \
      printf("TDBStore Key Retrieve Fail Code %d. Failed to get " #key         \
             ". Using fallback!",                                              \
             macroerr);                                                        \
      fflush(stdout);                                                          \
      *(configPtr) = fallback;                                                 \
    }                                                                          \
  }

/**
 * @brief Macro Save PID Configuration from TDBStore database
 * @param key Matching key to save to
 */
#define TRY_SAVE_PID(key, configPtr)                                           \
  {                                                                            \
    if (int macroerr = tdbStore.set(key, configPtr, sizeof(PID::Config), 0);   \
        macroerr != MBED_SUCCESS) {                                            \
      printf("TDBStore Key Save Fail Code %d. Failed to save to " #key,        \
             macroerr);                                                        \
      fflush(stdout);                                                          \
    }                                                                          \
  }

// See RM0368 Section 3.3 for details on flash memory layout.
// We occupy final 2 128 kb sectors of flash memory, which is more
// than I'd like, but this makes it very simple.
// initialize final 2 sectors (using offset of 0x08040000 with size 256 KiB )
static FlashIAPBlockDevice flash(0x08040000, 256 * 1024);
static TDBStore
    tdbStore(&flash); /**< Get/Set Interface for config value store. */

static BufferedSerial bleSerial(PA_11, PA_12, 9600); /**< hm10 serial */

static EventQueue mainQueue(32 * EVENTS_EVENT_SIZE); /**< User context defer */

FileHandle *mbed::mbed_override_console(int fd) { return &bleSerial; }

static DigitalOut enable(PB_2, 0);
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart2;

DigitalOut out(LED1);

void toggleController(uint8_t cmd) { enable.write(!enable.read()); }

void bleCommandSaveAllPID(uint8_t cmd) {}
PID::Config lsCfg;
Motor::Config motorLCfg = MOTORL_DEFAULT;
Motor::Config motorRCfg = MOTORR_DEFAULT;

LineSensor lineSensor(LS_21A_CFG);
Encoder eLeft(&htim3, 1ms);
Encoder eRight(&htim4, 1ms);
void Error_Handler(void) {
  while (1) {
    __NOP();
  }
}

void dbg() {
  printf("encSpeed1 %d \n", eRight.getSteps(), eRight.getSteps());
  fflush(stdout);
}
InterruptIn button(PB_1);
void updateEncoders(void) {}

void doOtherThing(void) {}

int main(void) {
  MX_TIM3_Init();
  MX_TIM4_Init();
  eLeft.start();
  eRight.start();
  int err = 0;
  if ((err = flash.init()) != 0) {
    DEBUG("Flash init failed: %d\n", err);
  }
  if ((err = tdbStore.init()) != 0) {
    DEBUG("TDBStore init failed: %d\n", err);
  }
  srand(time(NULL));
  Bluetooth bluetooth(
      &bleSerial, &mainQueue,
      std::vector<Bluetooth::Command>{
          // Demo LED Blink
          Bluetooth::Command('A', 0xFF,
                             callback([](uint8_t cmd) { out = !out; })),
          // Demo BLE Console Functionality
          Bluetooth::Command('B', 0xFF, callback([](uint8_t cmd) {
                               DEBUG("Shall we play a game?");
                             })),
          Bluetooth::Command(
              'O', 0xFF, callback([](uint8_t cmd) {
                int number = rand() % (INT_MAX - 100);
                if (int err = tdbStore.set("testKey", &number, sizeof(int), 0);
                    err != MBED_SUCCESS) {
                  DEBUG("test key fail %d ", err);
                } else {
                  DEBUG("Test Key Set Success \n");
                }
                int readback;
                if (int err = tdbStore.get("testKey", &readback, sizeof(int));
                    err != MBED_SUCCESS) {
                  DEBUG("test key retrieve fail %d \n", err);
                } else {
                  DEBUG("Test Key Get Success %d \n", readback);
                }
              })),
          Bluetooth::Command('R', 0xFF, callback([](uint8_t cmd) { // reset uC
                               NVIC_SystemReset();
                             })),
          Bluetooth::Command('P', 0xFF, callback([](uint8_t cmd) {
                               int readback;
                               if (int err = tdbStore.get("testKey", &readback,
                                                          sizeof(int));
                                   err != MBED_SUCCESS) {
                                 DEBUG("test key retrieve fail %d \n", err);
                               } else {
                                 DEBUG("Test Key Get Success %d \n", readback);
                               }
                             })),
      },
      true);
  Ticker ticker;
  ticker.attach(callback(&doOtherThing), 1s);
  button.rise(callback(
    []() { mainQueue.call(&updateEncoders); }
  ));
  mainQueue.call_every(1000, &updateEncoders);
  mainQueue.dispatch_forever();

  while (1) {
    updateEncoders();
    ThisThread::sleep_for(1s);
  }
}
