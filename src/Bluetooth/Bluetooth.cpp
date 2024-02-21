#include "Bluetooth.hpp"

Bluetooth::Bluetooth(PinName tx, PinName rx, PinName ledPin) : hm10(tx,rx,9600), led(ledPin) {}

void Bluetooth::checkData() {
        if (hm10.readable()) {
            cmd = hm10.read(&cmd, 1);
            if (cmd == 'A') {
                beepLED();
            }
        }
    }

void Bluetooth::beepLED() {
        led = 1;
        ThisThread::sleep_for(2ms);
        led = 0;
};

