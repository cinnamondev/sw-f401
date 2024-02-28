#ifndef _DS2871_HPP_
#define _DS2871_HPP_

#include "mbed.h"
#include "../onewire-stm32/OneWire.hpp"

class DS2871 : OneWire {
    Ticker batteryEventListener;
    float lowBatteryThreshold;
    Callback<void()> event_lowBattery;
    void eventTick(void);
    public:
    DS2871(USART_TypeDef* usart);
    /**
     * @brief Execute a callback 
     * 
     * @param e Callback to execute on low battery
     * @param pollRate Time between battery checks
     * @param targetVoltage Voltage required to determine 'low battery'
     */
    void onLowBattery(Callback<void()> e, std::chrono::milliseconds pollRate, float targetVoltage);
    /**
     * @brief Stop listening for low battery events
     * 
     */
    void stopListenLowBattery(void);
    float getVoltage(void);
    float getCurrent(void);
    float getCurrentAvg(void);
};

#endif // DS2871.hpp