#ifndef LINESENSOR_HPP
#define LINESENSOR_HPP

#include "mbed.h"

#define LS_21A_CFG (struct LineSensor::Config) { .s = {A0,A1,A2,A3,A4,A5} }
#define LS_21A_N 6
class LineSensor {
    public:
    struct Config {
        PinName s[6];
    };
    /**
     * @brief Create a line sensor target
     * @param config
     */
    LineSensor(Config config = LS_21A_CFG);
    float getLinePosition(void);
    private:
    unsigned short raw_buff[6];
    void updateSensors(void) {
      raw_buff[0] = s1.read_u16();
      raw_buff[1] = s2.read_u16();
      raw_buff[2] = s3.read_u16();
      raw_buff[3] = s5.read_u16();
      raw_buff[4] = s5.read_u16();
      raw_buff[5] = s6.read_u16();
    }
    AnalogIn s1;
    AnalogIn s2;
    AnalogIn s3;
    AnalogIn s4;
    AnalogIn s5;
    AnalogIn s6;
};

#ifdef TARGET_LSPCB_21A
static LineSensor lineSensor = LineSensor();
#endif
#endif // LINESENSOR_HPP