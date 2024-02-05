#ifndef __PLOTTER_INC_
#define __PLOTTER_INC_

#include "mbed.h"
#include "../c12832/C12832.h"

// plot some curve y(x) onto the display
class Plotter {
    C12832* display;
    // 128 samples along a curve
    static uint8_t plotYLevel[128];
    int xMin;
    int xMax;
    int rangeStep;
    public:
    Plotter(C12832* display, int xMinimum, int xMaximum);

};

#endif