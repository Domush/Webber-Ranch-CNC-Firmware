/**
 * Modern Vintage CNC Firmware
*/

/**
 * mvCNC RGB LED general support
 */

#include "../../inc/mvCNCConfig.h"

#if ENABLED(NEOPIXEL_LED)

#include "leds.h"

#if EITHER(NEOPIXEL_STARTUP_TEST, NEOPIXEL2_STARTUP_TEST)
  #include "../../core/utility.h"
#endif

mvCNC_NeoPixel neo;
pixel_index_t mvCNC_NeoPixel::neoindex;

Adafruit_NeoPixel mvCNC_NeoPixel::adaneo1(NEOPIXEL_PIXELS, NEOPIXEL_PIN, NEOPIXEL_TYPE + NEO_KHZ800);
#if CONJOINED_NEOPIXEL
  Adafruit_NeoPixel mvCNC_NeoPixel::adaneo2(NEOPIXEL_PIXELS, NEOPIXEL2_PIN, NEOPIXEL2_TYPE + NEO_KHZ800);
#endif

#ifdef NEOPIXEL_BKGD_INDEX_FIRST

  void mvCNC_NeoPixel::set_background_color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    for  (int background_led = NEOPIXEL_BKGD_INDEX_FIRST; background_led <= NEOPIXEL_BKGD_INDEX_LAST; background_led++)
      set_pixel_color(background_led, adaneo1.Color(r, g, b, w));
  }

  void mvCNC_NeoPixel::reset_background_color() {
    constexpr uint8_t background_color[4] = NEOPIXEL_BKGD_COLOR;
    set_background_color(background_color[0], background_color[1], background_color[2], background_color[3]);
  }

#endif

void mvCNC_NeoPixel::set_color(const uint32_t color) {
  if (neoindex >= 0) {
    set_pixel_color(neoindex, color);
    neoindex = -1;
  }
  else {
    for (uint16_t i = 0; i < pixels(); ++i) {
      #ifdef NEOPIXEL_BKGD_INDEX_FIRST
        if (i == NEOPIXEL_BKGD_INDEX_FIRST && TERN(NEOPIXEL_BKGD_ALWAYS_ON, true, color != 0x000000)) {
          reset_background_color();
          i += NEOPIXEL_BKGD_INDEX_LAST - (NEOPIXEL_BKGD_INDEX_FIRST);
          continue;
        }
      #endif
      set_pixel_color(i, color);
    }
  }
  show();
}

void mvCNC_NeoPixel::set_color_startup(const uint32_t color) {
  for (uint16_t i = 0; i < pixels(); ++i)
    set_pixel_color(i, color);
  show();
}

void mvCNC_NeoPixel::init() {
  neoindex = -1;                       // -1 .. NEOPIXEL_PIXELS-1 range
  set_brightness(NEOPIXEL_BRIGHTNESS); //  0 .. 255 range
  begin();
  show();  // initialize to all off

  #if ENABLED(NEOPIXEL_STARTUP_TEST)
    set_color_startup(adaneo1.Color(255, 0, 0, 0));  // red
    safe_delay(500);
    set_color_startup(adaneo1.Color(0, 255, 0, 0));  // green
    safe_delay(500);
    set_color_startup(adaneo1.Color(0, 0, 255, 0));  // blue
    safe_delay(500);
    #if HAS_WHITE_LED
      set_color_startup(adaneo1.Color(0, 0, 0, 255));  // white
      safe_delay(500);
    #endif
  #endif

  #ifdef NEOPIXEL_BKGD_INDEX_FIRST
    reset_background_color();
  #endif

  set_color(adaneo1.Color
    TERN(LED_USER_PRESET_STARTUP,
      (LED_USER_PRESET_RED, LED_USER_PRESET_GREEN, LED_USER_PRESET_BLUE, LED_USER_PRESET_WHITE),
      (0, 0, 0, 0))
  );
}

#if ENABLED(NEOPIXEL2_SEPARATE)

  mvCNC_NeoPixel2 neo2;

  pixel_index_t mvCNC_NeoPixel2::neoindex;
  Adafruit_NeoPixel mvCNC_NeoPixel2::adaneo(NEOPIXEL2_PIXELS, NEOPIXEL2_PIN, NEOPIXEL2_TYPE);

  void mvCNC_NeoPixel2::set_color(const uint32_t color) {
    if (neoindex >= 0) {
      set_pixel_color(neoindex, color);
      neoindex = -1;
    }
    else {
      for (uint16_t i = 0; i < pixels(); ++i)
        set_pixel_color(i, color);
    }
    show();
  }

  void mvCNC_NeoPixel2::set_color_startup(const uint32_t color) {
    for (uint16_t i = 0; i < pixels(); ++i)
      set_pixel_color(i, color);
    show();
  }

  void mvCNC_NeoPixel2::init() {
    neoindex = -1;                        // -1 .. NEOPIXEL2_PIXELS-1 range
    set_brightness(NEOPIXEL2_BRIGHTNESS); //  0 .. 255 range
    begin();
    show();  // initialize to all off

    #if ENABLED(NEOPIXEL2_STARTUP_TEST)
      set_color_startup(adaneo.Color(255, 0, 0, 0));  // red
      safe_delay(500);
      set_color_startup(adaneo.Color(0, 255, 0, 0));  // green
      safe_delay(500);
      set_color_startup(adaneo.Color(0, 0, 255, 0));  // blue
      safe_delay(500);
      #if HAS_WHITE_LED2
        set_color_startup(adaneo.Color(0, 0, 0, 255));  // white
        safe_delay(500);
      #endif
    #endif

    set_color(adaneo.Color
      TERN(NEO2_USER_PRESET_STARTUP,
        (NEO2_USER_PRESET_RED, NEO2_USER_PRESET_GREEN, NEO2_USER_PRESET_BLUE, NEO2_USER_PRESET_WHITE),
        (0, 0, 0, 0))
    );
  }

#endif // NEOPIXEL2_SEPARATE

#endif // NEOPIXEL_LED
