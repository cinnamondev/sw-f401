#ifndef SW_F401_ENCODER_HPP
#define SW_F401_ENCODER_HPP

#ifndef LIB_ENCODER_NOMBED
#include "mbed.h"
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_hal_tim.h"

/**
 * @brief STM32F4 HAL TIM Encoder Wrapper intended for MBED OS.
 *
 * @section Usage (TIcker/MBED)
 * 
 * 1. Use STM32CubeMX to generate the timer configuration code. (Refer to your micro's reference manual and CubeMX)
 * 2. Put the config code somewhere in your code. You need to call your MX_TIMx_Init code before using Encoder.
 * This includes putting the Timer MSP weak functions somewhere, for the HAL to use.
 * 3. Pass your TIM_HandleTypeDef you want to use to Encoder when initializing it.
 * 4. Start the Encoder using `start` when ready. There are 2 ways to obtain the speed, one is more reliable
 * while the other might hinder performance slightly. If you are able to be relatively optimistic that the
 * internal ticker can run regularly without large delays that change its time interval, use `getSteps`,
 * as it will be equivalent to the number of steps within `sampleRate`. Otherwise, use `getSpeed`, it will
 * it will return a speed in terms of `steps/millisecond` and uses the mbed microsecond ticker to get a time
 * stamp.
 * 
 * @section Usage (no ticker / standard codebase compatibility)
 * 
 * 0. Define the macro LIB_ENCODER_NOMBED to disable ticker/remove mbed dependency.
 * 
 * See above up to & including step 3.
 * 
 * 4. Regularly call `update` yourself. You must be able to call this regularly
 * somehow, or precisely determine the time between calls, as each call updates
 * the steps as the number of steps since last update call. If you dont have
 * mbed/use this feature, you will not be able to make use of `getSpeed`. 
 */
class Encoder
{
  #define LIB_ENCODER_NOMBED
public:
#ifndef LIB_ENCODER_NOMBED
  /**
   * @brief Construct a new Encoder object
   *
   * @param timer STM32 timer to use (see `Usage`.)
   */
  explicit Encoder(TIM_HandleTypeDef *timer, std::chrono::microseconds sampleRate);
#else
  explicit Encoder(TIM_HandleTypeDef* timer);
#endif
  /**
   * @brief Returns the number of steps since the previous 'tick'.
   *
   * @return unsigned int Number of steps since last tick.
   */
  int getSteps();
  /**
   * @brief Start the encoder ticker at the previously defined sample rate.
   */
#ifndef LIB_ENCODER_NOMBED
  void start();
  /**
   * @brief Stop the encoder ticker.
   */
  void stop();

private:
#endif
  /**
   * @brief Check for a change in steps counted from the timer. Call regularly (perhaps via a ticker or background event)
   * for a regular speed.
   *
   */
  void update();
#ifndef LIB_ENCODER_NOMBED
  Ticker ticker; /**< Mbed ticker to update encoder*/
  std::chrono::microseconds tickRate; /**< Rate to update encoders at*/
  uint32_t t1 = 0; /**< Previous time sample (mbed) */
#else
  private:
#endif
  TIM_HandleTypeDef *hwTimer; /**< STM32 HAL Timer Peripheral*/
  volatile int steps = 0; /**< Number of steps since last tick*/
  uint32_t c1 = 0;
};

#endif // SW_F401_ENCODER_HPP
