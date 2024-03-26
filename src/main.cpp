/**
 * @file main.cpp
 * @brief Line following buggy firmware
 * Line following buggy
 */

#include "mbed.h"
#include "TDBStore.h"
#include "FlashIAPBlockDevice.h"

#include "../lib/logging.h"
#include "Bluetooth.hpp"
#include "Encoder.hpp"
#include "Motor.hpp"
#include "Movement.hpp"

#include "ll_init.h"



static BufferedSerial bleSerial(PA_11, PA_12, 9600); /**< hm10 serial */

FileHandle *mbed::mbed_override_console(int fd) { return &bleSerial; }

// See RM0368 Section 3.3 for details on flash memory layout.
// We occupy final 2 128 kb sectors of flash memory, which is more
// than I'd like, but this makes it very simple.
static FlashIAPBlockDevice flash(0x08040000, 256 * 1024);
static TDBStore tdbStore(&flash);

static DigitalOut enable(PB_2,0);
static PwmOut pwmL(PC_7);
DigitalOut out(LED1);
void toggleLED(uint8_t cmd) {
  bool on = out.read();
  out.write(!on);
}

void pingPong(uint8_t cmd) {
  printf("Hello from BLE!");
  fflush(stdout);
  bleSerial.sync();
}

// example buggy control cmmands

void buggyStart(uint8_t cmd) {
  printf("buggy started");
  fflush(stdout);
}

void buggyStop(uint8_t cmd) {
  printf("buggy stopped");
  fflush(stdout);
}

void buggyTurnaround(uint8_t cmd) {
  printf("sequence added...");
  fflush(stdout);
}

void commandLinePosition(uint8_t cmd) {
  printf("command line position");
  fflush(stdout);
}

void toggleOutput(uint8_t cmd) {
  enable.write(!enable.read());
}

Queue<Movement, 16> movenentQueue;

int main(void) {
  pwmL.period_us(30);
  pwmL.write(0.6);

  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_TIM4_Init();

  int err = 0;
  if (err = flash.init() < 0) { DEBUG("Flash init failed: %d\n", err);}
  if (err = tdbStore.init() < 0) {DEBUG("TDBStore init failed: %d\n", err);}
  PID::Config motorLeftConfig;

  tdbStore.get("motorLeft", &motorLeftConfig, sizeof(motorLeftConfig));
  Bluetooth bluetooth(
      &bleSerial,
      std::vector<Bluetooth::Command>{
          Bluetooth::Command('A', 0xFF, callback(&toggleLED)), //  LED Test
          Bluetooth::Command('B', 0xFF,
                             callback(&pingPong)), // module response test
          Bluetooth::Command('S', 0xFF, callback(&buggyStart)),
          Bluetooth::Command('Z', 0xFF, callback(&buggyStop)),
          Bluetooth::Command('T', 0xFF, callback(&buggyTurnaround)),
          Bluetooth::Command('L', 0xFF, callback(&commandLinePosition)),
          Bluetooth::Command('P', 0xFF, callback(&toggleOutput)),
      },
      true);
  Encoder encoder(TIM3);
  while (true) {
    encoder.update();
    ThisThread::sleep_for(100ms);
    //Movement newMovement;
    //movenentQueue.try_get_for(Kernel::wait_for_u32_forever, &newMovement);
  }
}
