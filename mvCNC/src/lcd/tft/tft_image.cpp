/**
 * Modern Vintage CNC Firmware
*/

#include "../../inc/mvCNCConfigPre.h"

#if HAS_GRAPHICAL_TFT

#include "tft_image.h"
#include "ui_common.h"

const tImage NoLogo                 = { nullptr, 0, 0, NOCOLORS };

#if ENABLED(SHOW_BOOTSCREEN)
  const tImage mvCNCLogo112x38x1   = { (void *)mvcnc_logo_112x38x1, 112, 38, GREYSCALE1 };
  const tImage mvCNCLogo228x255x2  = { (void *)mvcnc_logo_228x255x2, 228, 255, GREYSCALE2 };
  const tImage mvCNCLogo228x255x4  = { (void *)mvcnc_logo_228x255x4, 228, 255, GREYSCALE4 };
  const tImage mvCNCLogo195x59x16  = { (void *)mvcnc_logo_195x59x16,  195,  59, HIGHCOLOR };
  const tImage mvCNCLogo320x240x16 = { (void *)mvcnc_logo_320x240x16, 320, 240, HIGHCOLOR };
  const tImage mvCNCLogo480x320x16 = { (void *)mvcnc_logo_480x320x16, 480, 320, HIGHCOLOR };
#endif
const tImage Background320x30x16    = { (void *)background_320x30x16, 320, 30, HIGHCOLOR };

const tImage HotEnd_64x64x4         = { (void *)hotend_64x64x4, 64, 64, GREYSCALE4 };
const tImage Bed_64x64x4            = { (void *)bed_64x64x4, 64, 64, GREYSCALE4 };
const tImage Bed_Heated_64x64x4     = { (void *)bed_heated_64x64x4, 64, 64, GREYSCALE4 };
const tImage Chamber_64x64x4        = { (void *)chamber_64x64x4, 64, 64, GREYSCALE4 };
const tImage Chamber_Heated_64x64x4 = { (void *)chamber_heated_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan0_64x64x4           = { (void *)fan0_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan1_64x64x4           = { (void *)fan1_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan_Slow0_64x64x4      = { (void *)fan_slow0_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan_Slow1_64x64x4      = { (void *)fan_slow1_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan_Fast0_64x64x4      = { (void *)fan_fast0_64x64x4, 64, 64, GREYSCALE4 };
const tImage Fan_Fast1_64x64x4      = { (void *)fan_fast1_64x64x4, 64, 64, GREYSCALE4 };
const tImage SD_64x64x4             = { (void *)sd_64x64x4, 64, 64, GREYSCALE4 };
const tImage Home_64x64x4           = { (void *)home_64x64x4, 64, 64, GREYSCALE4 };
const tImage BtnRounded_64x52x4     = { (void *)btn_rounded_64x52x4, 64, 52, GREYSCALE4 };
const tImage BtnRounded_42x39x4     = { (void *)btn_rounded_42x39x4, 42, 39, GREYSCALE4 };
const tImage Menu_64x64x4           = { (void *)menu_64x64x4, 64, 64, GREYSCALE4 };
const tImage Settings_64x64x4       = { (void *)settings_64x64x4, 64, 64, GREYSCALE4 };
const tImage Confirm_64x64x4        = { (void *)confirm_64x64x4, 64, 64, GREYSCALE4 };
const tImage Cancel_64x64x4         = { (void *)cancel_64x64x4, 64, 64, GREYSCALE4 };
const tImage Increase_64x64x4       = { (void *)increase_64x64x4, 64, 64, GREYSCALE4 };
const tImage Decrease_64x64x4       = { (void *)decrease_64x64x4, 64, 64, GREYSCALE4 };
const tImage Pause_64x64x4          = { (void *)pause_64x64x4, 64, 64, GREYSCALE4 };

const tImage Feedrate_32x32x4       = { (void *)feedrate_32x32x4, 32, 32, GREYSCALE4 };
const tImage Flowrate_32x32x4       = { (void *)flowrate_32x32x4, 32, 32, GREYSCALE4 };
const tImage Directory_32x32x4      = { (void *)directory_32x32x4, 32, 32, GREYSCALE4 };
const tImage Back_32x32x4           = { (void *)back_32x32x4, 32, 32, GREYSCALE4 };
const tImage Up_32x32x4             = { (void *)up_32x32x4, 32, 32, GREYSCALE4 };
const tImage Down_32x32x4           = { (void *)down_32x32x4, 32, 32, GREYSCALE4 };
const tImage Left_32x32x4           = { (void *)left_32x32x4, 32, 32, GREYSCALE4 };
const tImage Right_32x32x4          = { (void *)right_32x32x4, 32, 32, GREYSCALE4 };
const tImage Refresh_32x32x4        = { (void *)refresh_32x32x4, 32, 32, GREYSCALE4 };
const tImage Leveling_32x32x4       = { (void *)leveling_32x32x4, 32, 32, GREYSCALE4 };

const tImage Slider8x16x4           = { (void *)slider_8x16x4, 8, 16, GREYSCALE4 };

const tImage Images[imgCount] = {
  TERN(SHOW_BOOTSCREEN, TERN(BOOT_MVCNC_LOGO_SMALL, mvCNCLogo195x59x16, mvCNC_LOGO_FULL_SIZE), NoLogo),
  HotEnd_64x64x4,
  Bed_64x64x4,
  Bed_Heated_64x64x4,
  Chamber_64x64x4,
  Chamber_Heated_64x64x4,
  Fan0_64x64x4,
  Fan_Slow0_64x64x4,
  Fan_Slow1_64x64x4,
  Fan_Fast0_64x64x4,
  Fan_Fast1_64x64x4,
  Feedrate_32x32x4,
  Flowrate_32x32x4,
  SD_64x64x4,
  Menu_64x64x4,
  Settings_64x64x4,
  Directory_32x32x4,
  Confirm_64x64x4,
  Cancel_64x64x4,
  Increase_64x64x4,
  Decrease_64x64x4,
  Back_32x32x4,
  Up_32x32x4,
  Down_32x32x4,
  Left_32x32x4,
  Right_32x32x4,
  Refresh_32x32x4,
  Leveling_32x32x4,
  Slider8x16x4,
  Home_64x64x4,
  BtnRounded_64x52x4,
  BtnRounded_42x39x4,
};

#endif // HAS_GRAPHICAL_TFT
