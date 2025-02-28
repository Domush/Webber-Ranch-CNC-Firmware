/**
 * Modern Vintage CNC Firmware
*/
#pragma once

/**
 * Danish
 *
 * LCD Menu Messages
 * See also https://mvcncfw.org/docs/development/lcd_language.html
 */

#define DISPLAY_CHARSET_ISO10646_1

namespace Language_da {
  using namespace Language_en; // Inherit undefined strings from English

  constexpr uint8_t CHARSIZE              = 2;
  LSTR LANGUAGE                           = _UxGT("Danish");

  LSTR WELCOME_MSG                        = MACHINE_NAME _UxGT(" er klar");
  LSTR MSG_MEDIA_INSERTED                 = _UxGT("Kort isat");
  LSTR MSG_MEDIA_REMOVED                  = _UxGT("Kort fjernet");
  LSTR MSG_MAIN                           = _UxGT("Menu");
  LSTR MSG_DISABLE_STEPPERS               = _UxGT("Slå alle steppere fra");
  LSTR MSG_AUTO_HOME                      = _UxGT("Auto Home"); // G28
  LSTR MSG_LEVEL_BED_WAITING              = _UxGT("Klik når du er klar");
  LSTR MSG_LEVEL_BED_NEXT_POINT           = _UxGT("Næste punkt");
  LSTR MSG_LEVEL_BED_DONE                 = _UxGT("Bed level er færdig!");
  LSTR MSG_SET_HOME_OFFSETS               = _UxGT("Sæt forsk. af home");
  LSTR MSG_HOME_OFFSETS_APPLIED           = _UxGT("Forsk. er nu aktiv");
  LSTR MSG_SET_ORIGIN                     = _UxGT("Sæt origin");
  #if HAS_PREHEAT
    LSTR MSG_PREHEAT_1                    = _UxGT("Forvarm ") PREHEAT_1_LABEL;
    LSTR MSG_PREHEAT_1_H                  = _UxGT("Forvarm ") PREHEAT_1_LABEL " ~";
    LSTR MSG_PREHEAT_1_END                = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" end");
    LSTR MSG_PREHEAT_1_END_E              = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" end ~");
    LSTR MSG_PREHEAT_1_ALL                = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" Alle");
    LSTR MSG_PREHEAT_1_BEDONLY            = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" Bed");
    LSTR MSG_PREHEAT_1_SETTINGS           = _UxGT("Forvarm ") PREHEAT_1_LABEL _UxGT(" conf");

    LSTR MSG_PREHEAT_M                    = _UxGT("Forvarm $");
    LSTR MSG_PREHEAT_M_H                  = _UxGT("Forvarm $ ~");
    LSTR MSG_PREHEAT_M_END                = _UxGT("Forvarm $ end");
    LSTR MSG_PREHEAT_M_END_E              = _UxGT("Forvarm $ end ~");
    LSTR MSG_PREHEAT_M_ALL                = _UxGT("Forvarm $ Alle");
    LSTR MSG_PREHEAT_M_BEDONLY            = _UxGT("Forvarm $ Bed");
    LSTR MSG_PREHEAT_M_SETTINGS           = _UxGT("Forvarm $ conf");
  #endif
  LSTR MSG_COOLDOWN                       = _UxGT("Afkøl");
  LSTR MSG_SWITCH_PS_ON                   = _UxGT("Slå strøm til");
  LSTR MSG_SWITCH_PS_OFF                  = _UxGT("Slå strøm fra");
  LSTR MSG_EXTRUDE                        = _UxGT("Extruder");
  LSTR MSG_MOVE_AXIS                      = _UxGT("Flyt akser");
  LSTR MSG_BED_LEVELING                   = _UxGT("Juster bed");
  LSTR MSG_LEVEL_BED                      = _UxGT("Juster bed");
  LSTR MSG_MOVE_X                         = _UxGT("Flyt X");
  LSTR MSG_MOVE_Y                         = _UxGT("Flyt Y");
  LSTR MSG_MOVE_Z                         = _UxGT("Flyt Z");
  LSTR MSG_MOVE_N_MM                      = _UxGT("Flyt %smm");
  LSTR MSG_MOVE_01MM                      = _UxGT("Flyt 0.1mm");
  LSTR MSG_MOVE_1MM                       = _UxGT("Flyt 1mm");
  LSTR MSG_MOVE_10MM                      = _UxGT("Flyt 10mm");
  LSTR MSG_MOVE_100MM                     = _UxGT("Flyt 100mm");
  LSTR MSG_SPEED                          = _UxGT("Hastighed");
  LSTR MSG_BED_Z                          = _UxGT("Plade Z");
  LSTR MSG_NOZZLE                         = _UxGT("Dyse");
  LSTR MSG_NOZZLE_N                       = _UxGT("Dyse ~");

  LSTR MSG_BED                            = _UxGT("Plade");
  LSTR MSG_FAN_SPEED                      = _UxGT("Blæser hastighed");
  LSTR MSG_FAN_SPEED_N                    = _UxGT("Blæser hastighed ~");
  LSTR MSG_CONTROL                        = _UxGT("Kontrol");
  LSTR MSG_MIN                            = _UxGT(" \002 Min");
  LSTR MSG_MAX                            = _UxGT(" \002 Max");
  LSTR MSG_FACTOR                         = _UxGT(" \002 Fact");
  LSTR MSG_AUTOTEMP                       = _UxGT("Autotemp");
  LSTR MSG_LCD_ON                         = _UxGT("Til");
  LSTR MSG_LCD_OFF                        = _UxGT("Fra");
  LSTR MSG_SELECT                         = _UxGT("Vælg");
  LSTR MSG_SELECT_E                       = _UxGT("Vælg *");
  LSTR MSG_A_RETRACT                      = _UxGT("A-retract");
  LSTR MSG_A_TRAVEL                       = _UxGT("A-rejse");
  LSTR MSG_TEMPERATURE                    = _UxGT("Temperatur");
  LSTR MSG_MOTION                         = _UxGT("Bevægelse");
  LSTR MSG_FILAMENT                       = _UxGT("Filament");
  LSTR MSG_VOLUMETRIC_ENABLED             = _UxGT("E i mm") SUPERSCRIPT_THREE;
  LSTR MSG_FILAMENT_DIAM                  = _UxGT("Fil. Dia.");
  LSTR MSG_FILAMENT_DIAM_E                = _UxGT("Fil. Dia. *");
  LSTR MSG_CONTRAST                       = _UxGT("LCD kontrast");
  LSTR MSG_STORE_EEPROM                   = _UxGT("Gem i EEPROM");
  LSTR MSG_LOAD_EEPROM                    = _UxGT("Hent fra EEPROM");
  LSTR MSG_RESTORE_DEFAULTS               = _UxGT("Gendan Defaults");
  LSTR MSG_REFRESH                        = LCD_STR_REFRESH  _UxGT("Genopfrisk");
  LSTR MSG_INFO_SCREEN                    = _UxGT("Info skærm");
  LSTR MSG_PREPARE                        = _UxGT("Forbered");
  LSTR MSG_PAUSE_PRINT                    = _UxGT("Pause printet");
  LSTR MSG_RESUME_PRINT                   = _UxGT("Forsæt printet");
  LSTR MSG_STOP_PRINT                     = _UxGT("Stop printet");
  LSTR MSG_MEDIA_MENU                     = _UxGT("CNC fra SD");
  LSTR MSG_NO_MEDIA                       = _UxGT("Intet SD kort");
  LSTR MSG_DWELL                          = _UxGT("Dvale...");
  LSTR MSG_USERWAIT                       = _UxGT("Venter på bruger...");
  LSTR MSG_PRINT_ABORTED                  = _UxGT("Print annulleret");
  LSTR MSG_NO_MOVE                        = _UxGT("Ingen bevægelse.");
  LSTR MSG_KILLED                         = _UxGT("DRÆBT. ");
  LSTR MSG_STOPPED                        = _UxGT("STOPPET. ");
  LSTR MSG_CONTROL_RETRACT                = _UxGT("Tilbagetræk mm");
  LSTR MSG_CONTROL_RETRACT_SWAP           = _UxGT("Skift Re.mm");
  LSTR MSG_CONTROL_RETRACTF               = _UxGT("Tilbagetræk V");
  LSTR MSG_CONTROL_RETRACT_ZHOP           = _UxGT("Hop mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER        = _UxGT("Unretr. mm");
  LSTR MSG_CONTROL_RETRACT_RECOVER_SWAP   = _UxGT("Skift Unretr. mm");
  LSTR MSG_CONTROL_RETRACT_RECOVERF       = _UxGT("Unretract V");
  LSTR MSG_AUTORETRACT                    = _UxGT("Auto-Retract");
  LSTR MSG_FILAMENTCHANGE                 = _UxGT("Skift filament");
  LSTR MSG_FILAMENTCHANGE_E               = _UxGT("Skift filament *");
  LSTR MSG_CHANGE_MEDIA                   = _UxGT("Skift SD kort");
  LSTR MSG_ZPROBE_OUT                     = _UxGT("Probe udenfor plade");
  LSTR MSG_BLTOUCH_SELFTEST               = _UxGT("BLTouch Selv-Test");
  LSTR MSG_HOME_FIRST                     = _UxGT("Home %s%s%s først");
  LSTR MSG_HEATING_FAILED_LCD             = _UxGT("Opvarmning fejlet");
  LSTR MSG_ERR_REDUNDANT_TEMP             = _UxGT("Fejl: reserve temp");
  LSTR MSG_THERMAL_RUNAWAY                = _UxGT("Temp løber løbsk");
  LSTR MSG_ERR_MAXTEMP                    = _UxGT("Fejl: Maks temp");
  LSTR MSG_ERR_MINTEMP                    = _UxGT("Fejl: Min temp");
  LSTR MSG_HALTED                         = _UxGT("PRINTER STOPPET");
  LSTR MSG_PLEASE_RESET                   = _UxGT("Reset Venligst");
  LSTR MSG_HEATING                        = _UxGT("Opvarmer...");
  LSTR MSG_BED_HEATING                    = _UxGT("Opvarmer plade...");
  LSTR MSG_DELTA_CALIBRATE                = _UxGT("Delta Kalibrering");
  LSTR MSG_DELTA_CALIBRATE_X              = _UxGT("Kalibrer X");
  LSTR MSG_DELTA_CALIBRATE_Y              = _UxGT("Kalibrer Y");
  LSTR MSG_DELTA_CALIBRATE_Z              = _UxGT("Kalibrer Z");
  LSTR MSG_DELTA_CALIBRATE_CENTER         = _UxGT("Kalibrerings Center");

  LSTR MSG_INFO_MENU                      = _UxGT("Om CNC");
  LSTR MSG_INFO_BOARD_MENU                = _UxGT("Kort Info");
  LSTR MSG_INFO_THERMISTOR_MENU           = _UxGT("Thermistors");

  #if LCD_WIDTH >= 20 || HAS_DWIN_E3V2
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("Ant. CNCs");
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Færdige");
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Total print tid");
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Længste print");
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Total Extruderet");
  #else
    LSTR MSG_INFO_PRINT_COUNT             = _UxGT("CNCs");
    LSTR MSG_INFO_COMPLETED_PRINTS        = _UxGT("Færdige");
    LSTR MSG_INFO_PRINT_TIME              = _UxGT("Total");
    LSTR MSG_INFO_PRINT_LONGEST           = _UxGT("Længste");
    LSTR MSG_INFO_PRINT_FILAMENT          = _UxGT("Extruderet");
  #endif

  LSTR MSG_INFO_PSU                       = _UxGT("Strømfors.");

  LSTR MSG_DRIVE_STRENGTH                 = _UxGT("Driv Styrke");
  LSTR MSG_DAC_PERCENT_A                  = STR_A _UxGT(" Driv %");
  LSTR MSG_DAC_PERCENT_B                  = STR_B _UxGT(" Driv %");
  LSTR MSG_DAC_PERCENT_C                  = STR_C _UxGT(" Driv %");
  LSTR MSG_DAC_PERCENT_I                  = STR_I _UxGT(" Driv %");
  LSTR MSG_DAC_PERCENT_J                  = STR_J _UxGT(" Driv %");
  LSTR MSG_DAC_PERCENT_K                  = STR_K _UxGT(" Driv %");
  LSTR MSG_DAC_PERCENT_E                  = _UxGT("E Driv %");

  LSTR MSG_DAC_EEPROM_WRITE               = _UxGT("DAC EEPROM Skriv");

  LSTR MSG_FILAMENT_CHANGE_OPTION_RESUME  = _UxGT("Forsæt print");

  LSTR MSG_KILL_EXPECTED_PRINTER          = _UxGT("Forkert cnc");

  #if LCD_HEIGHT >= 4
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_3_LINE("Vent på start", "af filament", "skift"));
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_2_LINE("Vent på", "filament udskyd."));
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_3_LINE("Indsæt filament", "og tryk på knap", "for at fortsætte..."));
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_2_LINE("Vent på", "filament indtag"));
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_2_LINE("Vent på at print", "fortsætter"));
  #else // LCD_HEIGHT < 4
    LSTR MSG_FILAMENT_CHANGE_INIT         = _UxGT(MSG_1_LINE("Vent venligst..."));
    LSTR MSG_FILAMENT_CHANGE_UNLOAD       = _UxGT(MSG_1_LINE("Udskyder..."));
    LSTR MSG_FILAMENT_CHANGE_INSERT       = _UxGT(MSG_1_LINE("Indsæt og klik"));
    LSTR MSG_FILAMENT_CHANGE_LOAD         = _UxGT(MSG_1_LINE("Indtager..."));
    LSTR MSG_FILAMENT_CHANGE_RESUME       = _UxGT(MSG_1_LINE("Fortsætter..."));
  #endif // LCD_HEIGHT < 4
}
