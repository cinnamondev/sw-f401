#include "DS2871.hpp"

DS2871::DS2871(USART_TypeDef *usart) : OneWire(usart, 9600) {}

void DS2871::eventTick(void) {
    if(getVoltage() <= lowBatteryThreshold) {
        event_lowBattery();
    }
}
void DS2871::onLowBattery(Callback<void()> e, std::chrono::milliseconds polling,
                          float targetVoltage) {
    event_lowBattery = e;
    batteryEventListener.attach(callback(this, DS2871::eventTick), polling);
}

void DS2871::stopListenLowBattery(void) {
    batteryEventListener.detach();
    event_lowBattery = 0;
}

float DS2871::getVoltage(void) {
}
float DS2871::getCurrent(void) {}
float DS2871::getCurrentAvg(void) {}