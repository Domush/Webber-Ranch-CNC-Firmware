/**
 * Modern Vintage CNC Firmware
*/
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <cstring>

#include <pinmapping.h>

#define HIGH         0x01
#define LOW          0x00

#define INPUT          0x00
#define OUTPUT         0x01
#define INPUT_PULLUP   0x02
#define INPUT_PULLDOWN 0x03

#define LSBFIRST     0
#define MSBFIRST     1

#define CHANGE       0x02
#define FALLING      0x03
#define RISING       0x04

typedef uint8_t byte;
#define PROGMEM
#define PSTR(v) (v)
#define PGM_P const char *

// Used for libraries, preprocessor, and constants
#define abs(x) ((x)>0?(x):-(x))

#ifndef isnan
  #define isnan std::isnan
#endif
#ifndef isinf
  #define isinf std::isinf
#endif

#define sq(v) ((v) * (v))
#define square(v) sq(v)
#define constrain(value, arg_min, arg_max) ((value) < (arg_min) ? (arg_min) :((value) > (arg_max) ? (arg_max) : (value)))

//Interrupts
void cli(); // Disable
void sei(); // Enable
void attachInterrupt(uint32_t pin, void (*callback)(), uint32_t mode);
void detachInterrupt(uint32_t pin);

extern "C" {
  void GpioEnableInt(uint32_t port, uint32_t pin, uint32_t mode);
  void GpioDisableInt(uint32_t port, uint32_t pin);
}

// Time functions
extern "C" void delay(const int milis);
void _delay_ms(const int delay);
void delayMicroseconds(unsigned long);
uint32_t millis();

//IO functions
void pinMode(const pin_t, const uint8_t);
void digitalWrite(pin_t, uint8_t);
bool digitalRead(pin_t);
void analogWrite(pin_t, int);
uint16_t analogRead(pin_t);

int32_t random(int32_t);
int32_t random(int32_t, int32_t);
void randomSeed(uint32_t);

char *dtostrf(double __val, signed char __width, unsigned char __prec, char *__s);

int map(uint16_t x, uint16_t in_min, uint16_t in_max, uint16_t out_min, uint16_t out_max);
