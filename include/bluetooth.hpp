#include "mbed.h"
class BLUETOOTH {
    private:
        Ticker tic; 
        char c;
        BufferedSerial hm10 (D0,D1);
    
    public:
        BLUETOOTH(PinName T, PinName R) : hm10(T,R) {tic.attach(callback(this, &Blutooth::signal),0.001),hm10.baud(9600);}
        void signal (void){
            if (hm10.readable()){     // Checks if data is available
                c = hm10.getc();      // Store value hm10 in c
                if(c== 'A'){          // Checks if c is equal to A
                    turnaround();     // Callback, calls function turnaround
                }
            } 

        }

    


}
