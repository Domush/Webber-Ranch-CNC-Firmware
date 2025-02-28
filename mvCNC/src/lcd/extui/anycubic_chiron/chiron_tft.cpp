/**
 * Modern Vintage CNC Firmware
*/

/**
 * lcd/extui/anycubic_chiron/chiron_tft.cpp
 *
 * Extensible_UI implementation for Anycubic Chiron
 * Written By Nick Wells, 2020 [https://github.com/SwiftNick]
 *  (not affiliated with Anycubic, Ltd.)
 */

#include "../../../inc/mvCNCConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_CHIRON)

#include "chiron_tft.h"
#include "Tunes.h"
#include "FileNavigator.h"

#include "../../../gcode/queue.h"
#include "../../../module/stepper.h"
#include "../../../sd/cardreader.h"
#include "../../../libs/numtostr.h"
#include "../../../mvCNCCore.h"

namespace Anycubic {

ChironTFT Chiron;
#if AUTO_DETECT_CHIRON_TFT
  panel_type_t   ChironTFT::panel_type = AC_panel_unknown;
#endif
last_error_t     ChironTFT::last_error;
cnc_state_t  ChironTFT::cnc_state;
paused_state_t   ChironTFT::pause_state;
heater_state_t   ChironTFT::hotend_state;
heater_state_t   ChironTFT::hotbed_state;
xy_uint8_t       ChironTFT::selectedmeshpoint;
char             ChironTFT::selectedfile[MAX_PATH_LEN + 1];
char             ChironTFT::panel_command[MAX_CMND_LEN + 1];
uint8_t          ChironTFT::command_len;
float            ChironTFT::live_Zoffset;
file_menu_t      ChironTFT::file_menu;

void ChironTFT::Startup() {
  selectedfile[0]   = '\0';
  panel_command[0]  = '\0';
  command_len       = 0;
  last_error        = AC_error_none;
  cnc_state     = AC_cnc_idle;
  pause_state       = AC_paused_idle;
  hotend_state      = AC_heater_off;
  hotbed_state      = AC_heater_off;
  live_Zoffset      = 0.0;
  file_menu         = AC_menu_file;

  // Setup pins for powerloss detection
  // Two IO pins are connected on the Trigorilla Board
  // On a power interruption the OUTAGECON_PIN goes low.

  #if ENABLED(POWER_LOSS_RECOVERY)
    OUT_WRITE(OUTAGECON_PIN, HIGH);
  #endif

  // Filament runout is handled by mvCNC settings in Configuration.h
  // opt_set    FIL_RUNOUT_STATE HIGH  // Pin state indicating that filament is NOT present.
  // opt_enable FIL_RUNOUT_PULLUP
  TFTSer.begin(115200);

  // wait for the TFT panel to initialise and finish the animation
  delay_ms(250);

  // There are different panels for the Chiron with slightly different commands
  // So we need to know what we are working with.

  // Panel type can be defined otherwise detect it automatically
  if (panel_type == AC_panel_unknown) DetectPanelType();

  // Signal Board has reset
  SendtoTFTLN(AC_msg_main_board_has_reset);

  // Enable leveling and Disable end stops during print
  // as Z home places nozzle above the bed so we need to allow it past the end stops
  injectCommands(AC_cmnd_enable_leveling);

  // Startup tunes are defined in Tunes.h
  PlayTune(BEEPER_PIN, TERN(AC_DEFAULT_STARTUP_TUNE, Anycubic_PowerOn, GB_PowerOn), 1);

  #if ACDEBUGLEVEL
    SERIAL_ECHOLNPGM("AC Debug Level ", ACDEBUGLEVEL);
  #endif
  SendtoTFTLN(AC_msg_ready);
}

void ChironTFT::DetectPanelType() {
  #if AUTO_DETECT_CHIRON_TFT
    // Send a query to the TFT
    SendtoTFTLN(AC_Test_for_OldPanel); // The panel will respond with 'SXY 480 320'
    SendtoTFTLN(AC_Test_for_NewPanel); // the panel will respond with '[0]=0   ' to '[19]=0   '
  #endif
}

void ChironTFT::IdleLoop()  {
  if (ReadTFTCommand()) {
    ProcessPanelRequest();
    command_len = 0;
  }
  CheckHeaters();
}

void ChironTFT::CNCKilled(FSTR_P const error, FSTR_P const component)  {
  SendtoTFTLN(AC_msg_kill_lcd);
  #if ACDEBUG(AC_mvCNC)
    SERIAL_ECHOLNPGM("CNCKilled()\nerror: ", error , "\ncomponent: ", component);
  #endif
}

void ChironTFT::MediaEvent(media_event_t event)  {
  #if ACDEBUG(AC_mvCNC)
    SERIAL_ECHOLNPGM("ProcessMediaStatus() ", event);
  #endif
  switch (event) {
    case AC_media_inserted:
      SendtoTFTLN(AC_msg_sd_card_inserted);
      break;

    case AC_media_removed:
      SendtoTFTLN(AC_msg_sd_card_removed);
      break;

    case AC_media_error:
      last_error = AC_error_noSD;
      SendtoTFTLN(AC_msg_no_sd_card);
      break;
  }
}

void ChironTFT::TimerEvent(timer_event_t event)  {
  #if ACDEBUG(AC_mvCNC)
    SERIAL_ECHOLNPGM("TimerEvent() ", event);
    SERIAL_ECHOLNPGM("CNC State: ", cnc_state);
  #endif

  switch (event) {
    case AC_timer_started: {
      live_Zoffset = 0.0; // reset print offset
      setSoftEndstopState(false);  // disable endstops to print
      cnc_state = AC_cnc_job_running;
      SendtoTFTLN(AC_msg_print_from_sd_card);
    } break;

    case AC_timer_paused: {
      cnc_state = AC_cnc_paused;
      pause_state   = AC_paused_idle;
      SendtoTFTLN(AC_msg_paused);
    } break;

    case AC_timer_stopped: {
      if (cnc_state != AC_cnc_idle) {
        cnc_state = AC_cnc_stopping;
        SendtoTFTLN(AC_msg_print_complete);
      }
      setSoftEndstopState(true); // enable endstops
    } break;
  }
}

void ChironTFT::FilamentRunout()  {
  #if ACDEBUG(AC_mvCNC)
    SERIAL_ECHOLNPGM("FilamentRunout() cnc_state ", cnc_state);
  #endif
  // 1 Signal filament out
  last_error = AC_error_filament_runout;
  SendtoTFTLN(isPrintingFromMedia() ? AC_msg_filament_out_alert : AC_msg_filament_out_block);
  PlayTune(BEEPER_PIN, FilamentOut, 1);
}

void ChironTFT::ConfirmationRequest(const char * const msg)  {
  // M108 continue
  #if ACDEBUG(AC_mvCNC)
    SERIAL_ECHOLNPGM("ConfirmationRequest() ", msg, " cnc_state:", cnc_state);
  #endif
  switch (cnc_state) {
    case AC_cnc_pausing: {
      if (strcmp_P(msg, mvCNC_msg_print_paused) == 0 || strcmp_P(msg, mvCNC_msg_nozzle_parked) == 0) {
        SendtoTFTLN(AC_msg_paused); // enable continue button
        cnc_state = AC_cnc_paused;
      }
    } break;

    case AC_cnc_resuming_from_power_outage:
    case AC_cnc_job_running:
    case AC_cnc_paused: {
      // Heater timeout, send acknowledgement
      if (strcmp_P(msg, mvCNC_msg_heater_timeout) == 0) {
        pause_state = AC_paused_heater_timed_out;
        SendtoTFTLN(AC_msg_paused); // enable continue button
        PlayTune(BEEPER_PIN,Heater_Timedout,1);
      }
      // Reheat finished, send acknowledgement
      else if (strcmp_P(msg, mvCNC_msg_reheat_done) == 0) {
        pause_state = AC_paused_idle;
        SendtoTFTLN(AC_msg_paused); // enable continue button
      }
      // Filament Purging, send acknowledgement enter run mode
      else if (strcmp_P(msg, mvCNC_msg_filament_purging) == 0) {
        pause_state = AC_paused_purging_filament;
        SendtoTFTLN(AC_msg_paused); // enable continue button
      }
    } break;
    default:
    break;
  }
}

void ChironTFT::StatusChange(const char * const msg)  {
  #if ACDEBUG(AC_mvCNC)
    SERIAL_ECHOLNPGM("StatusChange() ", msg);
    SERIAL_ECHOLNPGM("cnc_state:", cnc_state);
  #endif
  bool msg_matched = false;
  // The only way to get cnc status is to parse messages
  // Use the state to minimise the work we do here.
  switch (cnc_state) {
    case AC_cnc_probing: {
      // If probing completes ok save the mesh and park
      // Ignore the custom machine name
      if (strcmp_P(msg + strlen(MACHINE_NAME), mvCNC_msg_ready) == 0) {
        injectCommands(F("M500\nG27"));
        SendtoTFTLN(AC_msg_probing_complete);
        cnc_state = AC_cnc_idle;
        msg_matched = true;
      }
      // If probing fails don't save the mesh raise the probe above the bad point
      if (strcmp_P(msg, mvCNC_msg_probing_failed) == 0) {
        PlayTune(BEEPER_PIN, BeepBeepBeeep, 1);
        injectCommands(F("G1 Z50 F500"));
        SendtoTFTLN(AC_msg_probing_complete);
        cnc_state = AC_cnc_idle;
        msg_matched = true;
      }
    } break;

    case AC_cnc_job_running: {
      if (strcmp_P(msg, mvCNC_msg_reheating) == 0) {
        SendtoTFTLN(AC_msg_paused); // enable continue button
        msg_matched = true;
       }
    } break;

    case AC_cnc_pausing: {
      if (strcmp_P(msg, mvCNC_msg_print_paused) == 0) {
        SendtoTFTLN(AC_msg_paused);
        cnc_state = AC_cnc_paused;
        pause_state = AC_paused_idle;
        msg_matched = true;
       }
    } break;

    case AC_cnc_stopping: {
      if (strcmp_P(msg, mvCNC_msg_print_aborted) == 0) {
        SendtoTFTLN(AC_msg_stop);
        cnc_state = AC_cnc_idle;
        msg_matched = true;
      }
    } break;
    default:
    break;
  }

  // If not matched earlier see if this was a heater message
  if (!msg_matched) {
    if (strcmp_P(msg, mvCNC_msg_extruder_heating) == 0) {
      SendtoTFTLN(AC_msg_nozzle_heating);
      hotend_state = AC_heater_temp_set;
    }
    else if (strcmp_P(msg, mvCNC_msg_bed_heating) == 0) {
      SendtoTFTLN(AC_msg_bed_heating);
      hotbed_state = AC_heater_temp_set;
    }
    else if (strcmp_P(msg, mvCNC_msg_EEPROM_version) == 0) {
      last_error = AC_error_EEPROM;
    }
  }
}

void ChironTFT::PowerLossRecovery()  {
  cnc_state = AC_cnc_resuming_from_power_outage; // Play tune to notify user we can recover.
  last_error = AC_error_powerloss;
  PlayTune(BEEPER_PIN, SOS, 1);
  SERIAL_ECHOLNF(AC_msg_powerloss_recovery);
}

void ChironTFT::CNCComplete() {
  SendtoTFT(AC_msg_print_complete);
  cnc_state = AC_cnc_idle;
  setSoftEndstopState(true); // enable endstops
}

void ChironTFT::SendtoTFT(FSTR_P const fstr/*=nullptr*/) {  // A helper to print PROGMEM string to the panel
  #if ACDEBUG(AC_SOME)
    SERIAL_ECHOF(fstr);
  #endif
  PGM_P str = FTOP(fstr);
  while (const char c = pgm_read_byte(str++)) TFTSer.write(c);
}

void ChironTFT::SendtoTFTLN(FSTR_P const fstr/*=nullptr*/) {
  if (fstr) {
    #if ACDEBUG(AC_SOME)
      SERIAL_ECHOPGM("> ");
    #endif
    SendtoTFT(fstr);
    #if ACDEBUG(AC_SOME)
      SERIAL_EOL();
    #endif
  }
  TFTSer.println();
}

bool ChironTFT::ReadTFTCommand() {
  bool command_ready = false;
  while (TFTSer.available() > 0 && command_len < MAX_CMND_LEN) {
    panel_command[command_len] = TFTSer.read();
    if (panel_command[command_len] == '\n') {
      command_ready = true;
      break;
    }
    command_len++;
  }

  if (command_ready || command_len == MAX_CMND_LEN) {
    panel_command[command_len] = '\0';
    #if ACDEBUG(AC_ALL)
      SERIAL_ECHOLNPGM("len(",command_len,") < ", panel_command);
    #endif
    command_ready = true;
  }
  return command_ready;
}

int8_t ChironTFT::FindToken(char c) {
  int8_t pos = 0;
  do {
    if (panel_command[pos] == c) {
      #if ACDEBUG(AC_INFO)
        SERIAL_ECHOLNPGM("Tpos:", pos, " ", c);
      #endif
      return pos;
    }
  } while (++pos < command_len);
  #if ACDEBUG(AC_INFO)
    SERIAL_ECHOLNPGM("Not found: ", c);
  #endif
  return -1;
}

void ChironTFT::CheckHeaters() {
  uint8_t faultDuration = 0;

  // if the hotend temp is abnormal, confirm state before signalling panel
  celsius_float_t temp = getActualTemp_celsius(E0);
  while (!WITHIN(temp, HEATER_0_MINTEMP, HEATER_0_MAXTEMP)) {
    faultDuration++;
    if (faultDuration >= AC_HEATER_FAULT_VALIDATION_TIME) {
      SendtoTFTLN(AC_msg_nozzle_temp_abnormal);
      last_error = AC_error_abnormal_temp_t0;
      SERIAL_ECHOLNPGM("Extruder temp abnormal! : ", temp);
      break;
    }
    delay_ms(500);
    temp = getActualTemp_celsius(E0);
  }

  // If the hotbed temp is abnormal, confirm state before signaling panel
  faultDuration = 0;
  temp = getActualTemp_celsius(BED);
  while (!WITHIN(temp, BED_MINTEMP, BED_MAXTEMP)) {
    faultDuration++;
    if (faultDuration >= AC_HEATER_FAULT_VALIDATION_TIME) {
      SendtoTFTLN(AC_msg_nozzle_temp_abnormal);
      last_error = AC_error_abnormal_temp_bed;
      SERIAL_ECHOLNPGM("Bed temp abnormal! : ", temp);
      break;
    }
    delay_ms(500);
    temp = getActualTemp_celsius(E0);
  }

  // Update panel with hotend heater status
  if (hotend_state != AC_heater_temp_reached) {
    if (WITHIN(getActualTemp_celsius(E0) - getTargetTemp_celsius(E0), -(TEMP_WINDOW), TEMP_WINDOW)) {
      SendtoTFTLN(AC_msg_nozzle_heating_done);
      hotend_state = AC_heater_temp_reached;
    }
  }

  // Update panel with bed heater status
  if (hotbed_state != AC_heater_temp_reached) {
    if (WITHIN(getActualTemp_celsius(BED) - getTargetTemp_celsius(BED), -(TEMP_BED_WINDOW), TEMP_BED_WINDOW)) {
      SendtoTFTLN(AC_msg_bed_heating_done);
      hotbed_state = AC_heater_temp_reached;
    }
  }
}

void ChironTFT::SendFileList(int8_t startindex) {
  // Respond to panel request for 4 files starting at index
  #if ACDEBUG(AC_INFO)
    SERIAL_ECHOLNPGM("## SendFileList ## ", startindex);
  #endif
  SendtoTFTLN(F("FN "));
  filenavigator.getFiles(startindex, panel_type, 4);
  SendtoTFTLN(F("END"));
}

void ChironTFT::SelectFile() {
  if (panel_type == AC_panel_new) {
    strncpy(selectedfile, panel_command + 4, command_len - 3);
    selectedfile[command_len - 4] = '\0';
  }
  else {
    strncpy(selectedfile, panel_command + 4, command_len - 4);
    selectedfile[command_len - 5] = '\0';
  }
  #if ACDEBUG(AC_FILE)
    SERIAL_ECHOLNPGM(" Selected File: ",selectedfile);
  #endif
  switch (selectedfile[0]) {
    case '/':   // Valid file selected
      SendtoTFTLN(AC_msg_sd_file_open_success);
      break;

    case '<':   // .. (go up folder level)
      filenavigator.upDIR();
      SendtoTFTLN(AC_msg_sd_file_open_failed);
      SendFileList( 0 );
      break;
    default:   // enter sub folder
      // for new panel remove the '.GCO' tag that was added to the end of the path
      if (panel_type == AC_panel_new)
        selectedfile[strlen(selectedfile) - 4] = '\0';
      filenavigator.changeDIR(selectedfile);
      SendtoTFTLN(AC_msg_sd_file_open_failed);
      SendFileList( 0 );
      break;
  }
}

void ChironTFT::ProcessPanelRequest() {
  // Break these up into logical blocks // as its easier to navigate than one huge switch case!
  int8_t tpos = FindToken('A');
  // Panel request are 'A0' - 'A36'
  if (tpos != -1) {
    const int8_t req = atoi(&panel_command[tpos+1]);

    // Information requests A0 - A8 and A33
    if (req <= 8 || req == 33) PanelInfo(req);

    // Simple Actions A9 - A28
    else if (req <= 28) PanelAction(req);

    // Process Initiation
    else if (req <= 36) PanelProcess(req);
  }
  else {
    #if AUTO_DETECT_CHIRON_TFT
      // This may be a response to a panel type detection query
      if (panel_type == AC_panel_unknown) {
        tpos = FindToken('S'); // old panel will respond to 'SIZE' with 'SXY 480 320'
        if (tpos != -1) {
          if (panel_command[tpos+1]== 'X' && panel_command[tpos+2]=='Y') {
            panel_type = AC_panel_standard;
            SERIAL_ECHOLNF(AC_msg_old_panel_detected);
          }
        }
        else {
          tpos = FindToken('['); // new panel will respond to 'J200' with '[0]=0'
          if (tpos != -1) {
            if (panel_command[tpos+1]== '0' && panel_command[tpos+2]==']') {
              panel_type = AC_panel_new;
              SERIAL_ECHOLNF(AC_msg_new_panel_detected);
            }
          }
        }
        return;
      }
    #endif

    SendtoTFTLN(); // Ignore unknown requests
  }
}

void ChironTFT::PanelInfo(uint8_t req) {
  // information requests A0-A8 and A33
  switch (req) {
    case 0:   // A0 Get HOTEND Temp
      SendtoTFT(F("A0V "));
      TFTSer.println(getActualTemp_celsius(E0));
      break;

    case 1:   // A1 Get HOTEND Target Temp
      SendtoTFT(F("A1V "));
      TFTSer.println(getTargetTemp_celsius(E0));
      break;

    case 2:   // A2 Get BED Temp
      SendtoTFT(F("A2V "));
      TFTSer.println(getActualTemp_celsius(BED));
      break;

    case 3:   // A3 Get BED Target Temp
      SendtoTFT(F("A3V "));
      TFTSer.println(getTargetTemp_celsius(BED));
      break;

    case 4:   // A4 Get FAN Speed
      SendtoTFT(F("A4V "));
      TFTSer.println(getActualFan_percent(FAN0));
      break;

    case 5:   // A5 Get Current Coordinates
      SendtoTFT(F("A5V X: "));
      TFTSer.print(getAxisPosition_mm(X));
      SendtoTFT(F(" Y: "));
      TFTSer.print(getAxisPosition_mm(Y));
      SendtoTFT(F(" Z: "));
      TFTSer.println(getAxisPosition_mm(Z));
      break;

    case 6:   // A6 Get job_running progress
      if (isPrintingFromMedia()) {
        SendtoTFT(F("A6V "));
        TFTSer.println(ui8tostr2(getProgress_percent()));
      }
      else
        SendtoTFTLN(F("A6V ---"));
      break;

    case 7: { // A7 Get CNCing Time
      uint32_t time = getProgress_seconds_elapsed() / 60;
      SendtoTFT(F("A7V "));
      TFTSer.print(ui8tostr2(time / 60));
      SendtoTFT(F(" H "));
      TFTSer.print(ui8tostr2(time % 60));
      SendtoTFT(F(" M"));
      #if ACDEBUG(AC_ALL)
        SERIAL_ECHOLNPGM("Print time ", ui8tostr2(time / 60), ":", ui8tostr2(time % 60));
      #endif
    } break;

    case 8:   // A8 Get SD Card list A8 S0
      if (!isMediaInserted()) safe_delay(500);
      if (!isMediaInserted())   // Make sure the card is removed
        SendtoTFTLN(AC_msg_no_sd_card);
      else if (panel_command[3] == 'S')
        SendFileList( atoi( &panel_command[4] ) );
      break;

    case 33:   // A33 Get firmware info
      SendtoTFT(F("J33 "));
      // If there is an error recorded, show that instead of the FW version
      if (!GetLastError()) SendtoTFTLN(F(SHORT_BUILD_VERSION));
      break;
  }
}

void ChironTFT::PanelAction(uint8_t req) {
  switch (req) {
    case  9:   // A9 Pause SD print
      if (isPrintingFromMedia()) {
        SendtoTFTLN(AC_msg_pause);
        pausePrint();
        cnc_state = AC_cnc_pausing;
      }
      else
        SendtoTFTLN(AC_msg_stop);
      break;

    case 10: // A10 Resume SD Print
      if (pause_state == AC_paused_idle || cnc_state == AC_cnc_resuming_from_power_outage)
        resumePrint();
      else
        setUserConfirmed();
      break;

    case 11:   // A11 Stop SD print
      if (isPrintingFromMedia()) {
        cnc_state = AC_cnc_stopping;
        stopPrint();
      }
      else {
        if (cnc_state == AC_cnc_resuming_from_power_outage)
          injectCommands(F("M1000 C")); // Cancel recovery
        SendtoTFTLN(AC_msg_stop);
        cnc_state = AC_cnc_idle;
      }
      break;

    case 12:   // A12 Kill cnc
      kill();  // from mvcnccore.h
      break;

    case 13:   // A13 Select file
      SelectFile();
      break;

    case 14: { // A14 Start CNCing
      // Allows cnc to restart the job if we don't want to recover
      if (cnc_state == AC_cnc_resuming_from_power_outage) {
        injectCommands(F("M1000 C")); // Cancel recovery
        cnc_state = AC_cnc_idle;
      }
      #if ACDebugLevel >= 1
        SERIAL_ECHOLNPAIR_F("CNC: ", selectedfile);
      #endif
      printFile(selectedfile);
      SendtoTFTLN(AC_msg_print_from_sd_card);
    } break;

    case 15:   // A15 Resuming from outage
      if (cnc_state == AC_cnc_resuming_from_power_outage) {
        // Need to home here to restore the Z position
        injectCommands(AC_cmnd_power_loss_recovery);
        injectCommands(F("M1000"));  // home and start recovery
      }
      break;

    case 16: { // A16 Set HotEnd temp  A17 S170
      const float set_Htemp = atof(&panel_command[5]);
      hotend_state = set_Htemp ? AC_heater_temp_set : AC_heater_off;
      switch ((char)panel_command[4]) {
        // Set Temp
        case 'S': case 'C': setTargetTemp_celsius(set_Htemp, E0);
      }
    } break;

    case 17: { // A17 Set bed temp
      const float set_Btemp = atof(&panel_command[5]);
      hotbed_state = set_Btemp ? AC_heater_temp_set : AC_heater_off;
      if (panel_command[4] == 'S')
        setTargetTemp_celsius(set_Btemp, BED);
    } break;

    case 18:   // A18 Set Fan Speed
      if (panel_command[4] == 'S')
        setTargetFan_percent(atof(&panel_command[5]), FAN0);
      break;

    case 19:   // A19 Motors off
      if (!isPrinting()) {
        stepper.disable_all_steppers();
        SendtoTFTLN(AC_msg_ready);
      }
      break;

    case 20:   // A20 Read/write print speed
      if (panel_command[4] == 'S')
        setFeedrate_percent(atoi(&panel_command[5]));
      else {
        SendtoTFT(F("A20V "));
        TFTSer.println(getFeedrate_percent());
      }
      break;

    case 21:   // A21 Home Axis  A21 X
      if (!isPrinting()) {
        switch ((char)panel_command[4]) {
          case 'X': injectCommands(F("G28X")); break;
          case 'Y': injectCommands(F("G28Y")); break;
          case 'Z': injectCommands(F("G28Z")); break;
          case 'C': injectCommands_P(G28_STR); break;
        }
      }
      break;

    case 22: {   // A22 Move Axis
      // The commands have changed on the new panel
      // Old TFT A22 X -1F1500      A22 X +1F1500
      // New TFT A22 X-1.0 F1500    A22 X1.0 F1500

      // lets just wrap this in a gcode relative nonprint move and let the controller deal with it
      // G91 G0 <panel command> G90

      if (!isPrinting()) { // Ignore request if running job
        char MoveCmnd[30];
        sprintf_P(MoveCmnd, PSTR("G91\nG0%s\nG90"), panel_command + 3);
        #if ACDEBUG(AC_ACTION)
          SERIAL_ECHOLNPGM("Move: ", MoveCmnd);
        #endif
        setSoftEndstopState(true);  // enable endstops
        injectCommands(MoveCmnd);
      }
    } break;

    case 23:   // A23 Preheat PLA
      // Ignore request if running job
      if (!isPrinting()) {
        // Temps defined in configuration.h
        setTargetTemp_celsius(PREHEAT_1_TEMP_BED, BED);
        setTargetTemp_celsius(PREHEAT_1_TEMP_HOTEND, E0);
        SendtoTFTLN();
        hotbed_state = AC_heater_temp_set;
        hotend_state = AC_heater_temp_set;
      }
      break;

    case 24:   // A24 Preheat ABS
      // Ignore request if running job
      if (!isPrinting()) {
        setTargetTemp_celsius(PREHEAT_2_TEMP_BED, BED);
        setTargetTemp_celsius(PREHEAT_2_TEMP_HOTEND, E0);
        SendtoTFTLN();
        hotbed_state = AC_heater_temp_set;
        hotend_state = AC_heater_temp_set;
      }
      break;

    case 25:   // A25 Cool Down
      // Ignore request if running job
      if (!isPrinting()) {
        setTargetTemp_celsius(0, E0);
        setTargetTemp_celsius(0, BED);
        SendtoTFTLN(AC_msg_ready);
        hotbed_state = AC_heater_off;
        hotend_state = AC_heater_off;
      }
      break;

    case 26:   // A26 Refresh SD
      if (card.isMounted())card.release();
      card.mount();
      safe_delay(500);
      filenavigator.reset();
      break;

    case 27:   // A27 Servo Angles adjust
      break;

    case 28:   // A28 Filament set A28 O/C
      // Ignore request if running job
      if (isPrinting()) break;
      SendtoTFTLN();
      break;
  }
}

void ChironTFT::PanelProcess(uint8_t req) {
  switch (req) {
    case 29: { // A29 Read Mesh Point A29 X1 Y1
      xy_uint8_t pos;
      float pos_z;
      pos.x = atoi(&panel_command[FindToken('X')+1]);
      pos.y = atoi(&panel_command[FindToken('Y')+1]);
      pos_z = getMeshPoint(pos);

      SendtoTFT(F("A29V "));
      TFTSer.println(pos_z * 100);
      if (!isPrinting()) {
        setSoftEndstopState(true);  // disable endstops
        // If the same meshpoint is selected twice in a row, move the head to that ready for adjustment
        if ((selectedmeshpoint.x == pos.x) && (selectedmeshpoint.y == pos.y)) {
          if (!isPositionKnown())
            injectCommands_P(G28_STR); // home

          if (isPositionKnown()) {
            #if ACDEBUG(AC_INFO)
              SERIAL_ECHOLNPGM("Moving to mesh point at x: ", pos.x, " y: ", pos.y, " z: ", pos_z);
            #endif
            // Go up before moving
            setAxisPosition_mm(3.0,Z);

            setAxisPosition_mm(17 + (93 * pos.x), X);
            setAxisPosition_mm(20 + (93 * pos.y), Y);
            setAxisPosition_mm(0.0, Z);
            #if ACDEBUG(AC_INFO)
              SERIAL_ECHOLNPGM("Current Z: ", getAxisPosition_mm(Z));
            #endif
          }
        }
        selectedmeshpoint.x = pos.x;
        selectedmeshpoint.y = pos.y;
      }
    } break;

    case 30: {   // A30 Auto leveling
      if (FindToken('S') != -1) { // Start probing New panel adds spaces..
        // Ignore request if running job
        if (isPrinting())
          SendtoTFTLN(AC_msg_probing_not_allowed); // forbid auto leveling
        else {


          SendtoTFTLN(AC_msg_start_probing);
          injectCommands(F("G28\nG29"));
          cnc_state = AC_cnc_probing;
        }
      }
      else {
        SendtoTFTLN(AC_msg_start_probing); // Just enter levelling menu
      }
    } break;

    case 31: { // A31 Adjust all Probe Points
      // The tokens can occur in different places on the new panel so we need to find it.

      if (FindToken('C') != -1) { // Restore and apply original offsets
        if (!isPrinting()) {
          injectCommands(F("M501\nM420 S1"));
          selectedmeshpoint.x = selectedmeshpoint.y = 99;
          SERIAL_ECHOLNF(AC_msg_mesh_changes_abandoned);
        }
      }

      else if (FindToken('D') != -1) { // Save Z Offset tables and restore leveling state
        if (!isPrinting()) {
          setAxisPosition_mm(1.0,Z); // Lift nozzle before any further movements are made
          injectCommands(F("M500"));
          SERIAL_ECHOLNF(AC_msg_mesh_changes_saved);
          selectedmeshpoint.x = selectedmeshpoint.y = 99;
        }
      }

      else if (FindToken('G') != -1) { // Get current offset
        SendtoTFT(F("A31V "));
        // When running job use the live z Offset position
        // we will use babystepping to move the print head
        if (isPrinting())
          TFTSer.println(live_Zoffset);
        else {
          TFTSer.println(getZOffset_mm());
          selectedmeshpoint.x = selectedmeshpoint.y = 99;
        }
      }

      else {
        int8_t tokenpos = FindToken('S');
        if (tokenpos != -1) { // Set offset (adjusts all points by value)
          float Zshift = atof(&panel_command[tokenpos+1]);
          setSoftEndstopState(false);  // disable endstops
          // Allow temporary Z position nudging during print
          // From the leveling panel use the all points UI to adjust the print pos.
          if (isPrinting()) {
            #if ACDEBUG(AC_INFO)
              SERIAL_ECHOLNPGM("Change Zoffset from:", live_Zoffset, " to ", live_Zoffset + Zshift);
            #endif
            if (isAxisPositionKnown(Z)) {
              #if ACDEBUG(AC_INFO)
                const float currZpos = getAxisPosition_mm(Z);
                SERIAL_ECHOLNPGM("Nudge Z pos from ", currZpos, " to ", currZpos + constrain(Zshift, -0.05, 0.05));
              #endif
              // Use babystepping to adjust the head position
              int16_t steps = mmToWholeSteps(constrain(Zshift,-0.05,0.05), Z);
              #if ACDEBUG(AC_INFO)
                SERIAL_ECHOLNPGM("Steps to move Z: ", steps);
              #endif
              babystepAxis_steps(steps, Z);
              live_Zoffset += Zshift;
            }
            SendtoTFT(F("A31V "));
            TFTSer.println(live_Zoffset);
          }
          else {
            GRID_LOOP(x, y) {
              const xy_uint8_t pos { x, y };
              const float currval = getMeshPoint(pos);
              setMeshPoint(pos, constrain(currval + Zshift, AC_LOWEST_MESHPOINT_VAL, 2));
              #if ACDEBUG(AC_INFO)
                SERIAL_ECHOLNPGM("Change mesh point X", x," Y",y ," from ", currval, " to ", getMeshPoint(pos) );
              #endif
            }
            const float currZOffset = getZOffset_mm();
            #if ACDEBUG(AC_INFO)
              SERIAL_ECHOLNPGM("Change probe offset from ", currZOffset, " to  ", currZOffset + Zshift);
            #endif

            setZOffset_mm(currZOffset + Zshift);
            SendtoTFT(F("A31V "));
            TFTSer.println(getZOffset_mm());

            if (isAxisPositionKnown(Z)) {
              // Move Z axis
              const float currZpos = getAxisPosition_mm(Z);
              #if ACDEBUG(AC_INFO)
                SERIAL_ECHOLNPGM("Move Z pos from ", currZpos, " to ", currZpos + constrain(Zshift, -0.05, 0.05));
              #endif
              setAxisPosition_mm(currZpos+constrain(Zshift,-0.05,0.05),Z);
            }
          }
        }
      }
    } break;

    case 32: { // A32 clean leveling beep flag
      // Ignore request if running job
      //if (isPrinting()) break;
      //injectCommands(F("M500\nM420 S1\nG1 Z10 F240\nG1 X0 Y0 F6000"));
      //TFTSer.println();
    } break;

    // A33 firmware info request see PanelInfo()

    case 34: {  // A34 Adjust single mesh point A34 C/S X1 Y1 V123
      if (panel_command[3] == 'C') { // Restore original offsets
        injectCommands(F("M501\nM420 S1"));
        selectedmeshpoint.x = selectedmeshpoint.y = 99;
        //cnc_state = AC_cnc_idle;
      }
      else {
        xy_uint8_t pos;
        pos.x = atoi(&panel_command[5]);
        pos.y = atoi(&panel_command[8]);

        float currmesh = getMeshPoint(pos);
        float newval   = atof(&panel_command[11])/100;
        #if ACDEBUG(AC_INFO)
          SERIAL_ECHOLNPGM("Change mesh point x:", pos.x, " y:", pos.y);
          SERIAL_ECHOLNPGM("from ", currmesh, " to ", newval);
        #endif
        // Update Meshpoint
        setMeshPoint(pos,newval);
        if (cnc_state == AC_cnc_idle || cnc_state == AC_cnc_probing /*!isPrinting()*/) {
          // if we are at the current mesh point indicated on the panel Move Z pos +/- 0.05mm
          // (The panel changes the mesh value by +/- 0.05mm on each button press)
          if (selectedmeshpoint.x == pos.x && selectedmeshpoint.y == pos.y) {
            setSoftEndstopState(false);
            float currZpos = getAxisPosition_mm(Z);
            #if ACDEBUG(AC_INFO)
              SERIAL_ECHOLNPGM("Move Z pos from ", currZpos, " to ", currZpos + constrain(newval - currmesh, -0.05, 0.05));
            #endif
            setAxisPosition_mm(currZpos + constrain(newval - currmesh, -0.05, 0.05), Z);
          }
        }
      }
    }  break;

    case 36:    // A36 Auto leveling for new TFT bet that was a typo in the panel code!
      SendtoTFTLN(AC_msg_start_probing);
      break;
  }
}

bool ChironTFT::GetLastError() {
  switch (last_error) {
    case AC_error_abnormal_temp_bed: SendtoTFTLN(AC_msg_error_bed_temp);    break;
    case AC_error_abnormal_temp_t0:  SendtoTFTLN(AC_msg_error_hotend_temp); break;
    case AC_error_noSD:              SendtoTFTLN(AC_msg_error_sd_card);     break;
    case AC_error_powerloss:         SendtoTFTLN(AC_msg_power_loss);        break;
    case AC_error_EEPROM:            SendtoTFTLN(AC_msg_eeprom_version);    break;
    case AC_error_filament_runout:   SendtoTFTLN(AC_msg_filament_out);      break;
    default: return false;
  }
  last_error = AC_error_none;
  return true;
}

} // Anycubic namespace

#endif // ANYCUBIC_LCD_CHIRON
