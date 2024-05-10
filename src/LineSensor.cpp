#include "LineSensor.hpp"

struct arr_range {
  size_t min;
  size_t max;
} RANGE_DEF = {
    .min = 0xFFFF,
    .max = 0xFFFF,
};

/**
 * @brief Traverse the buffer until one or more values above a threshold are
 * located. Returned value will be a range of array indicies that correspond to
 * the first consecutive group of values located above this threshold. 
 * 
 * @param buffer Buffer to traverse
 * @param size Size of buffer (or number of elements to search)
 * @param thres Threshold value to cross
 * @return struct arr_range Range of index values (.min = 0xFFFF indicates 
 * Not Found).
 */
static struct arr_range find_active_cluster(unsigned short *buffer, size_t size,
                                            unsigned short thres);
/**
 * @brief Find max value in buffer
 * 
 * @param buffer Buffer 
 * @param size Size of buffer
 * @return * size_t Index corresponding to max value
 */
static size_t find_max(unsigned short *buffer, size_t size);

LineSensor::LineSensor(LineSensor::Config config)
    : s1(config.s[0]), s2(config.s[1]), s3(config.s[2]),
      s4(config.s[3]), s5(config.s[4]), s6(config.s[5]) {}

float LineSensor::getLinePosition() {
  float linePosition = 0;
  updateSensors();
  struct arr_range rangeCons =
      find_active_cluster(raw_buff, 6, 40000);

  /*
  struct arr_range rangSPass = find_next_thres_cluster(raw_buff +
  rangeConsecutive.max, LINE_SENSOR_N-rangeConsecutive.max, THRESHOLD); if
  (rangeSPass.min != 0xFFFF) {
    // we have a problem...
  }
  */

  if (rangeCons.min == 0xFFFF) {
    // Not on the line - indicate line break!
    linePosition = -1;
  }
  size_t range = rangeCons.max - rangeCons.min;
  if (range > 3) {
    // strange edge case, we should never see this ever happen! probably a
    // turn... the behaviour will be to report the most extreme from centre
    // point. (with a preference to the right side if it cannot decide)
    bool closeToEdgeL = (rangeCons.min <= 2);
    bool closeToEdgeR = (rangeCons.max >= 5);
    if (closeToEdgeR) {
      linePosition = rangeCons.max;
    } else if (closeToEdgeL) {
      linePosition = rangeCons.min;
    } else {
      linePosition = rangeCons.max;
    }
  } else if (range == 3) {
    // find max within bounds
    size_t max = find_max(&raw_buff[rangeCons.min],
                          6 - rangeCons.min);

  } else if (range == 2) {
    // Find "where" the mean lies between the two (express mean as a percentage
    // increase of min then apply to indices) note: looks bad.

    float mean =
        (raw_buff[rangeCons.min] + raw_buff[rangeCons.max]) /
        2.00;

    float decrease = 100.0f - ((1.0f * raw_buff[rangeCons.max]) - mean) /
                                  (1.0f * raw_buff[rangeCons.max]);

    uint32_t max =
        (raw_buff[rangeCons.min] > raw_buff[rangeCons.max])
            ? rangeCons.min
            : rangeCons.max;
    bool minLarger =
        (raw_buff[rangeCons.min] > raw_buff[rangeCons.max]);
    linePosition =
        (raw_buff[rangeCons.min] > raw_buff[rangeCons.max])
            ? rangeCons.min + (range * decrease)
            : rangeCons.max - (range * decrease);
  } else {
    linePosition = rangeCons.min;
  }
  return linePosition;
}

static struct arr_range find_active_cluster(unsigned short *buffer, size_t size,
                                                unsigned short thres) {
  struct arr_range range;
  for (size_t i = 0; i < size; i++) {
    if (buffer[i] >= thres) {
      range.min = i;
      break;
    }
  }
  if (range.min == 0xFFFF) {
    return range;
  }
  for (size_t i = range.min + 1; i < size; i++) {
    if (buffer[i] <= thres) {
      range.max = i - 1;
      break;
    }
  }
  return range;
}

static size_t find_max(unsigned short *buffer, size_t size) {
  size_t i_m = 0;
  for (int i = 0; i < size; i++) {
    if (buffer[i] > buffer[i_m]) {
      i_m = i;
    }
  }
  return i_m;
}