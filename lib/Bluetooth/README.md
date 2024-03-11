```c
static BufferedSerial bleSerial(PA_11, PA_12, 9600);
DigitalOut out(LED1);
void toggleLED(uint8_t cmd) {
  bool on = out.read();
  out.write(!on);
}

int main(void) {
  Bluetooth bluetooth(
      &bleSerial,
      std::vector<Bluetooth::Command>{
          Bluetooth::Command('A', 0xFF, callback(&toggleLED)), //  LED Test
      },
      false);
  bluetooth.start();
  while (true);
}
```