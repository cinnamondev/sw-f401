#ifndef LOGGING_H
#define LOGGING_H

/** Print debug statement (immediate execution, will crash in ISR context, etc!)
 */
#define DEBUG(...)                                                             \
  printf(__VA_ARGS__);                                                         \
  fflush(stdout)

#ifdef __cplusplus

/** Print debug statement (deferred execution, use in time sensitive or
 * restricted sections) */
#define DEF_DEBUG(...)                                                         \
  mbed_event_queue()->call(callback([]() {                                     \
    printf(__VA_ARGS__);                                                       \
    fflush(stdout);                                                            \
  }))

#endif

#endif // LOGGING_H