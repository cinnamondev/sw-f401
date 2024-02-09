#ifndef __LINESENSOR_INC_
#define __LINESENSOR_INC_

#include "mbed.h"

class LineSensor {
    I2C bus;
    public:
    LineSensor(PinName sda, PinName scl);
    float getPosition(void);
};

#endif

// i2c bus command scheme
// 0x20 i2c address
//
// MSB high will always send the latest frame 
// (will process a new frame if freeze frame is enabled)
// 1XXXXXXXb - send latest frame
// 4th LSB is freeze frame control
// XXXX1XXXb - freeze frame processing
// XXXX0XXXb - unfreeze frame processing
// 3rd and 2nd LSB is frame debug data 
// XXXXX1XXb - send this frames raw data 
// XXXXXX1Xb - send this frames prediction steps 

// repeated calls to data + freeze frame commands should result
// in a 'step', where one sensor reading 