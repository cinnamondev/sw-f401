/**
 * @file defaultConfigs.hpp
 * @author cinnamondev
 * @brief Default fallback controller values (incase of flash fail)
 *
 */

#ifndef DEFAULTCONFIGS_HPP
#define DEFAULTCONFIGS_HPP

/**
 * @brief Line Sensor controller PID fallback values (if TDBStore fails).
 * @todo Update with tuned values
 */
#define LS_PID_FALLBACK                                                        \
  (struct PID::Config) {                                                       \
    .kp = 0.1f,                                                                \
    .ki = 0.1f,                                                                \
    .kd = 0.1f,                                                                \
    .in_min = -1.5f,                                                           \
    .in_max = 1.5f,                                                            \
    .out_min = -2.0f,                                                          \
    .out_max = 2.0f,                                                           \
    .sp = 0.0f,                                                                \
  }

/**
 * @brief Left motor default configuration with *NO* PID config. Source from
 * TDBStore or MOTORL_PID_FALLBACK.
 */
#define MOTORL_DEFAULT                                                         \
  (struct Motor::Config) {                                                     \
    .pwm = PC_7, .current = PC_3, .pid = MOTORL_PID_FALLBACK                     \
  }

/**
 * @brief Left Motor PID controller fallback values (if TDBStore fails).
 * @todo Update with tuned values
 */
#define MOTORL_PID_FALLBACK                                                    \
  (struct PID::Config) {                                                       \
    .kp = 0.1f,                                                                \
    .ki = 0.1f,                                                                \
    .kd = 0.1f,                                                                \
    .in_min = -1.5f,                                                           \
    .in_max = 1.5f,                                                            \
    .out_min = -2.0f,                                                          \
    .out_max = 2.0f,                                                           \
    .sp = 0.0f,                                                                \
  }

/**
 * @brief Left motor default configuration with *NO* PID config. Source from
 * TDBStore or MOTORR_PID_FALLBACK.
 */
#define MOTORR_DEFAULT                                                         \
  (struct Motor::Config) {                                                     \
    .pwm = PA_8, .current = PC_2, .pid = MOTORR_PID_FALLBACK                     \
  }

/**
 * @brief Right Motor PID controller fallback values (if TDBStore fails).
 * @todo Update with tuned values
 */
#define MOTORR_PID_FALLBACK                                                    \
  (struct PID::Config) {                                                       \
    .kp = 0.1f,                                                                \
    .ki = 0.1f,                                                                \
    .kd = 0.1f,                                                                \
    .in_min = -1.5f,                                                           \
    .in_max = 1.5f,                                                            \
    .out_min = -2.0f,                                                          \
    .out_max = 2.0f,                                                           \
    .sp = 0.0f,                                                                \
  }

#endif // DEFAULTCONFIGS_HPP
