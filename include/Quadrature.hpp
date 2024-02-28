#ifndef QUADRATUREHPP_
#define QUADRATUREHPP_
#include <cmath>
#include "stm32f4xx.h"
#include "mbed.h"

#define QUADRATURE_K_STP2DEG (360.0/255.0)

float stepsToDegrees(unsigned int stepsPerRevolution, unsigned int steps);
int degreesToSteps(float degrees, int stepsPerRevolution);
int distanceToSteps(float distanceMM, float diameterMM, int stepsPerRevolution);

/**
 * \brief Generic Quadrature encoder
 *
 * Implementations using different encoders may want to redefine
 * QUADRATURE_K_STP2DEG if their encoder is not 256 CPR.
 *
 * \warning Platform-dependent! This implementation uses the STM32CUBEF4 HAL,
 * so that the TIMER peripheral could be used for this application directly.
 */
class Quadrature {
  TIM_HandleTypeDef encoder; /** STM32 Timer */
  Callback<void()> stepsEvent = nullptr;
  unsigned long stepsEventThreshold = 0;
  bool runStepsEvent = false;
  unsigned long stepsTaken = 0; /** Note for future impls: this always increases regardless of direction. */
  float stepSpeed = 0;
public:
  /**
   * @brief Construct a new Quadrature object
   *
   * @param timer STM32 Timer Peripheral (STM32CUBEF4 HAL)
   *
   * It is expected that you will have already initialised GPIO for both timer
   * channels, do so prior to object initialization.
   */
  explicit Quadrature(TIM_TypeDef *timer);
  /**
   * \brief Updates the speed & step count of the encoder.
   *
   * If the distance event threshold is reached, it will be executed via this.
   * Code using this class should frequently call this method, as the name implies.
   * \note The first output of the 'ticker' cannot be considered accurate, as there is no
   * previous time data to work off. There is always going to be *some* error as it relies on
   * the MBed kernel tick clock, so it can only work on a resolution that is 1 ms, at best.
   */
  void tick();
  /**
   * \brief Get the direction that the quadrature encoder is turning in
   *
   * \note This function's output is NOT tied to the `tick` function,
   * and can function without `tick` ever being called. Though if you only need the direction,
   * it's reccomended to not bother with the abstraction and use the HAL directly.
   *
   * \return true Counting up (forwards?)
   * \return false Counting down (backwards?)
   */
  bool getDirection(void);
 // NOTE TO SELF: getSteps should not be accessible? only distanceEvents.
  /**
   * \brief Returns number of steps taken cumulatively.
   *
   * The number of steps taken
   * \note This does not pay regard to turning direction, it will increase regardless of rotation direction.
   * \return Number of steps taken.
   */
  unsigned long getCurrentSteps(void) { return stepsTaken;}
  /**
   * \brief Returns the speed of the shaft in "encoder steps".
   *
   * If you want this in degrees or beyond, you need to know the resolution of
   * your encoder.
   *
   * \return Speed of encoder in 'steps'/second.
   */
  float getCurrentSpeed(void) { return stepSpeed;}
  /**
   * \brief Returns a reference to the internal speed counter.
   * \note Unless you really-really need to, c
   * @return
   */
  const float* getSpeed(void) const { return &stepSpeed; } // im really not a fan.
  /**
   * \brief Registers a steps event.
   * \note Upon reaching the number of step
   * \param cb Callback to function to execute when a number of steps is
   * reached. \param steps Number of steps to reach
   */
  void onStepsReached(Callback<void()> cb, unsigned long steps);
  /**
   * \brief Disables the execution of the steps event callback ('hold in reset')
   */
  void pauseStepsEvent(void) { runStepsEvent = false; }
  /**
   * \brief Enables the execution of the steps event
   */
  void resumeStepsEvent(void) { runStepsEvent = false; }
  /**
   * \brief Unregisters a steps event
   */
  void unregisterStepsEvent(void);
  void ready(void) { HAL_TIM_Encoder_Start(&encoder, TIM_CHANNEL_ALL);}
  void stopTimer(void) { HAL_TIM_Encoder_Stop(&encoder, TIM_CHANNEL_ALL); }
};
#endif // QUADRATUREHPP_