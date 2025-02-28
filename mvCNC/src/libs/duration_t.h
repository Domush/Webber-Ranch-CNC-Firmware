/**
 * Modern Vintage CNC Firmware
*/
#pragma once

#include "../HAL/shared/Marduino.h"

struct duration_t {
  /**
   * @brief Duration is stored in seconds
   */
  uint32_t value;

  /**
   * @brief Constructor
   */
  duration_t()
    : duration_t(0) {};

  /**
   * @brief Constructor
   *
   * @param seconds The number of seconds
   */
  duration_t(uint32_t const &seconds) {
    this->value = seconds;
  }

  /**
   * @brief Equality comparison
   * @details Overloads the equality comparison operator
   *
   * @param value The number of seconds to compare to
   * @return True if both durations are equal
   */
  bool operator==(const uint32_t &value) const {
    return (this->value == value);
  }

  /**
   * @brief Inequality comparison
   * @details Overloads the inequality comparison operator
   *
   * @param value The number of seconds to compare to
   * @return False if both durations are equal
   */
  bool operator!=(const uint32_t &value) const {
    return ! this->operator==(value);
  }

  /**
   * @brief Formats the duration as years
   * @return The number of years
   */
  inline uint8_t year() const {
    return this->day() / 365;
  }

  /**
   * @brief Formats the duration as days
   * @return The number of days
   */
  inline uint16_t day() const {
    return this->hour() / 24;
  }

  /**
   * @brief Formats the duration as hours
   * @return The number of hours
   */
  inline uint32_t hour() const {
    return this->minute() / 60;
  }

  /**
   * @brief Formats the duration as minutes
   * @return The number of minutes
   */
  inline uint32_t minute() const {
    return this->second() / 60;
  }

  /**
   * @brief Formats the duration as seconds
   * @return The number of seconds
   */
  inline uint32_t second() const {
    return this->value;
  }

  #pragma GCC diagnostic push
  #if GCC_VERSION <= 50000
    #pragma GCC diagnostic ignored "-Wformat-overflow"
  #endif

  /**
   * @brief Formats the duration as a string
   * @details String will be formatted using a "full" representation of duration
   *
   * @param buffer The array pointed to must be able to accommodate 22 bytes
   *               (21 for the string, 1 more for the terminating nul)
   *
   * Output examples:
   *  123456789012345678901 (strlen)
   *  135y 364d 23h 59m 59s
   *  364d 23h 59m 59s
   *  23h 59m 59s
   *  59m 59s
   *  59s
   */
  char* toString(char * const buffer) const {
    const uint16_t y = this->year(),
                   d = this->day() % 365,
                   h = this->hour() % 24,
                   m = this->minute() % 60,
                   s = this->second() % 60;

         if (y) sprintf_P(buffer, PSTR("%iy %id %ih %im %is"), y, d, h, m, s);
    else if (d) sprintf_P(buffer, PSTR("%id %ih %im %is"), d, h, m, s);
    else if (h) sprintf_P(buffer, PSTR("%ih %im %is"), h, m, s);
    else if (m) sprintf_P(buffer, PSTR("%im %is"), m, s);
    else sprintf_P(buffer, PSTR("%is"), s);
    return buffer;
  }

  /**
   * @brief Formats the duration as a string
   * @details String will be formatted using a "digital" representation of duration
   *
   * @param buffer The array pointed to must be able to accommodate 10 bytes
   *
   * Output examples:
   *  123456789 (strlen)
   *  12'34
   *  99:59
   *  11d 12:33
   */
  uint8_t toDigital(char *buffer, bool with_days=false) const {
    const uint16_t h = uint16_t(this->hour()),
                   m = uint16_t(this->minute() % 60UL);
    if (with_days) {
      const uint16_t d = this->day();
      sprintf_P(buffer, PSTR("%hud %02hu:%02hu"), d, h % 24, m);  // 1d 23:45
      return d >= 10 ? 9 : 8;
    }
    else if (!h) {
      const uint16_t s = uint16_t(this->second() % 60UL);
      sprintf_P(buffer, PSTR("%02hu'%02hu"), m, s);     // 12'34
      return 5;
    }
    else if (h < 100) {
      sprintf_P(buffer, PSTR("%02hu:%02hu"), h, m);     // 12:34
      return 5;
    }
    else {
      sprintf_P(buffer, PSTR("%hu:%02hu"), h, m);       // 123:45
      return 6;
    }
  }

  #pragma GCC diagnostic pop
};
