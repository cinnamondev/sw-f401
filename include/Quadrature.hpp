#ifndef __QUADRATUREHPP_
#define __QUADRATUREHPP_
#include "mbed.h"
#include "STM32F4xx_HAL_Driver/stm32f4xx_hal_tim.h"
#include "STM32F4xx_HAL_Driver/stm32f4xx_hal_gpio.h"
class Quadrature {
    bool active = false;
    TIM_HandleTypeDef encoder;
    TIM_Encoder_InitTypeDef encoderCfg;
    float degs = 0;
    unsigned long encT1 = 0;
    Ticker encoderRead;
    /**
     * @brief Tick the rotary encoder.
     * 
     */
    void tick(void);
    public:
    /**
     * @brief Construct a new Quadrature object
     * 
     * @param timer STM32 Timer Peripheral (STM32CUBEF4 HAL)
     * 
     * It is expected that you will have already initialised GPIO for both timer channels,
     * do so prior to object initialization.
     */
    Quadrature(TIM_TypeDef* timer);
    /**
     * @brief Get the direction that the quadrature encoder is turning in
     * 
     * @return true Counting up (forwards?)
     * @return false Counting down (backwards?)
     */
    bool getDirection(void);
    /**
     * @brief Starts the ticker to read the speed of the encoder.
     * 
     */
    void start(void);
    /**
     * @brief Stops the ticker to read the speed of the encoder.
     * 
     */
    void stop(void);
    void reset(void);
    float rotarySpeed(void);
    bool isActive(void);
    static float toForwardSpeed(float wheelDiameter);
    void distanceEvent(Callback<void()> e, float)

};
#endif