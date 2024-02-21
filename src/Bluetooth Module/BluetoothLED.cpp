#include "mbed.h"

class Bluetooth {
private:
    Serial hm10;   
    DigitalOut myled; 
    char c;         

public:
    Bluetooth(PinName T, PinName R, PinName ledPin) : hm10(T, R), myled(ledPin) {
        hm10.baud(9600);
    }

    void checkData() {
        if (hm10.readable()) {
            c = hm10.getc(); 
            if (c == 'A') {
                ledOn(); // Turn on the LED
            }
        }
    }

    void ledOn() {
        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2); 
    }
};

int main() {
    Bluetooth bt(PA_11,PA_12, D4);

    while (true) {
        bt.checkData();  // Continuously check for data
        wait (0.02);
    }
}
