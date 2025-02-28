/**
 * Modern Vintage CNC Firmware
*/

#include "../../../inc/mvCNCConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include <lv_conf.h>

  #include "../../../module/pwm_temp_io.h"
  #include "../../../inc/mvCNCConfig.h"

static lv_obj_t *scr;
extern lv_group_t*  g;
static lv_obj_t *buttonType, *buttonStep, *buttonAdd, *buttonDec;
static lv_obj_t *labelType;
static lv_obj_t *labelStep;
static lv_obj_t *tempText1;
static lv_obj_t *btn_pla;
static lv_obj_t *btn_abs;
static lv_obj_t *label_abs;
static lv_obj_t *label_pla;

static lv_style_t btn_style_pre;
static lv_style_t btn_style_rel;

enum {
  ID_P_ADD = 1,
  ID_P_DEC,
  ID_P_TYPE,
  ID_P_STEP,
  ID_P_OFF,
  ID_P_RETURN,
  ID_P_ABS,
  ID_P_PLA
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_P_ADD: {
      if (uiCfg.curTempType == 0) {
        #if HAS_HOTEND
          int16_t max_target;
          fanManager.temp_hotend[uiCfg.extruderIndex].target += uiCfg.stepHeat;
          if (uiCfg.extruderIndex == 0)
            max_target = HEATER_0_MAXTEMP - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1);
          else {
            #if HAS_MULTI_HOTEND
              max_target = HEATER_1_MAXTEMP - (WATCH_TEMP_INCREASE + TEMP_HYSTERESIS + 1);
            #endif
          }
          if (fanManager.degTargetHotend(uiCfg.extruderIndex) > max_target)
            fanManager.setTargetHotend(max_target, uiCfg.extruderIndex);
          fanManager.start_watching_hotend(uiCfg.extruderIndex);
        #endif
      }
      else {
        #if HAS_HEATED_BED
          constexpr int16_t max_target = BED_MAXTEMP - (WATCH_BED_TEMP_INCREASE + TEMP_BED_HYSTERESIS + 1);
          fanManager.temp_bed.target += uiCfg.stepHeat;
          if (fanManager.degTargetBed() > max_target)
            fanManager.setTargetBed(max_target);
          fanManager.start_watching_bed();
        #endif
      }
      disp_desire_temp();
    } break;

    case ID_P_DEC:
      if (uiCfg.curTempType == 0) {
        #if HAS_HOTEND
        if (fanManager.degTargetHotend(uiCfg.extruderIndex) > uiCfg.stepHeat)
          fanManager.temp_hotend[uiCfg.extruderIndex].target -= uiCfg.stepHeat;
          else
            fanManager.setTargetHotend(0, uiCfg.extruderIndex);
          fanManager.start_watching_hotend(uiCfg.extruderIndex);
        #endif
      }
      else {
        #if HAS_HEATED_BED
        if (fanManager.degTargetBed() > uiCfg.stepHeat)
          fanManager.temp_bed.target -= uiCfg.stepHeat;
          else
            fanManager.setTargetBed(0);
          fanManager.start_watching_bed();
        #endif
      }
      disp_desire_temp();
      break;
    case ID_P_TYPE:
      if (uiCfg.curTempType == 0) {
        if (ENABLED(HAS_MULTI_EXTRUDER)) {
          if (uiCfg.extruderIndex == 0) {
            uiCfg.extruderIndex = 1;
          }
          else if (uiCfg.extruderIndex == 1) {
            if (ENABLED(HAS_HEATED_BED)) {
              uiCfg.curTempType = 1;
            }
            else {
              uiCfg.curTempType = 0;
              uiCfg.extruderIndex = 0;
            }
          }
        }
        else if (uiCfg.extruderIndex == 0) {
          uiCfg.curTempType = TERN(HAS_HEATED_BED, 1, 0);
        }
      }
      else if (uiCfg.curTempType == 1) {
        uiCfg.extruderIndex = 0;
        uiCfg.curTempType = 0;
      }
      disp_temp_type();
      break;
    case ID_P_STEP:
      switch (uiCfg.stepHeat) {
        case  1: uiCfg.stepHeat =  5; break;
        case  5: uiCfg.stepHeat = 10; break;
        case 10: uiCfg.stepHeat =  1; break;
        default: break;
      }
      disp_step_heat();
      break;
    case ID_P_OFF:
      if (uiCfg.curTempType == 0) {
        #if HAS_HOTEND
        fanManager.setTargetHotend(0, uiCfg.extruderIndex);
        fanManager.start_watching_hotend(uiCfg.extruderIndex);
        #endif
      }
      else {
        #if HAS_HEATED_BED
        fanManager.temp_bed.target = 0;
        fanManager.start_watching_bed();
        #endif
      }
      disp_desire_temp();
      break;
    case ID_P_RETURN:
      goto_previous_ui();
      break;
    case ID_P_ABS:
      if (uiCfg.curTempType == 0) {
        TERN_(HAS_HOTEND, fanManager.setTargetHotend(PREHEAT_2_TEMP_HOTEND, 0));
      }
      else if (uiCfg.curTempType == 1) {
        TERN_(HAS_HEATED_BED, fanManager.setTargetBed(PREHEAT_2_TEMP_BED));
      }
      break;
    case ID_P_PLA:
      if (uiCfg.curTempType == 0) {
        TERN_(HAS_HOTEND, fanManager.setTargetHotend(PREHEAT_1_TEMP_HOTEND, 0));
      }
      else if (uiCfg.curTempType == 1) {
        TERN_(HAS_HEATED_BED, fanManager.setTargetBed(PREHEAT_1_TEMP_BED));
      }
      break;
  }
}

void disp_add_dec() {
  // Create image buttons
  buttonAdd = lv_big_button_create(scr, "F:/bmp_Add.bin", preheat_menu.add, INTERVAL_V, titleHeight, event_handler, ID_P_ADD);
  buttonDec = lv_big_button_create(scr, "F:/bmp_Dec.bin", preheat_menu.dec, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_P_DEC);
}

void lv_draw_preHeat() {
  scr = lv_screen_create(PREHEAT_UI);

  // Create image buttons
  disp_add_dec();

  buttonType = lv_imgbtn_create(scr, nullptr, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_P_TYPE);
  buttonStep = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_P_STEP);

  if (uiCfg.curTempType == 0) disp_ext_heart();
  if (uiCfg.curTempType == 1) disp_ext_heart();

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonType);
      lv_group_add_obj(g, buttonStep);
    }
  #endif

  lv_big_button_create(scr, "F:/bmp_speed0.bin", preheat_menu.off, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_P_OFF);
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_P_RETURN);

  // Create labels on the image buttons
  labelType = lv_label_create_empty(buttonType);
  labelStep = lv_label_create_empty(buttonStep);

  disp_temp_type();
  disp_step_heat();

  tempText1 = lv_label_create_empty(scr);
  lv_obj_set_style(tempText1, &tft_style_label_rel);
  disp_desire_temp();
}

void disp_ext_heart() {
  btn_abs = lv_btn_create(scr, 160, 40, 80, 40, event_handler, ID_P_ABS);
  btn_pla = lv_btn_create(scr, 260, 40, 80, 40, event_handler, ID_P_PLA);

  lv_btn_set_style(btn_abs, LV_BTN_STYLE_PR, &btn_style_pre);
  lv_btn_set_style(btn_abs, LV_BTN_STYLE_REL, &btn_style_rel);
  lv_btn_set_style(btn_pla, LV_BTN_STYLE_PR, &btn_style_pre);
  lv_btn_set_style(btn_pla, LV_BTN_STYLE_REL, &btn_style_rel);

  label_abs = lv_label_create(btn_abs, PREHEAT_2_LABEL);
  label_pla = lv_label_create(btn_pla, PREHEAT_1_LABEL);
}

void disp_temp_type() {
  if (uiCfg.curTempType == 0) {
    if (TERN0(HAS_MULTI_EXTRUDER, uiCfg.extruderIndex == 1)) {
      lv_imgbtn_set_src_both(buttonType, "F:/bmp_extru2.bin");
      if (gCfgItems.multiple_language) {
        lv_label_set_text(labelType, preheat_menu.ext2);
        lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      }
    }
    else if (ENABLED(HAS_HOTEND)) {
      lv_imgbtn_set_src_both(buttonType, "F:/bmp_extru1.bin");
      if (gCfgItems.multiple_language) {
        lv_label_set_text(labelType, preheat_menu.ext1);
        lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      }
    }
  }
  else if (ENABLED(HAS_HEATED_BED)) {
    lv_imgbtn_set_src_both(buttonType, "F:/bmp_bed.bin");
    if (gCfgItems.multiple_language) {
      lv_label_set_text(labelType, preheat_menu.hotbed);
      lv_obj_align(labelType, buttonType, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void disp_desire_temp() {
  char buf[20] = { 0 };
  public_buf_l[0] = '\0';

  if (uiCfg.curTempType == 0) {
    #if HAS_HOTEND
      strcat(public_buf_l, uiCfg.extruderIndex < 1 ? preheat_menu.ext1 : preheat_menu.ext2);
      sprintf(buf, preheat_menu.value_state, fanManager.wholeDegHotend(uiCfg.extruderIndex), fanManager.degTargetHotend(uiCfg.extruderIndex));
    #endif
  }
  else {
    #if HAS_HEATED_BED
      strcat(public_buf_l, preheat_menu.hotbed);
      sprintf(buf, preheat_menu.value_state, fanManager.wholeDegBed(), fanManager.degTargetBed());
    #endif
  }
  strcat_P(public_buf_l, PSTR(": "));
  strcat(public_buf_l, buf);
  lv_label_set_text(tempText1, public_buf_l);
  lv_obj_align(tempText1, nullptr, LV_ALIGN_CENTER, 0, -50);
}

void disp_step_heat() {
  if (uiCfg.stepHeat == 1) {
    lv_imgbtn_set_src_both(buttonStep, "F:/bmp_step1_degree.bin");
  }
  else if (uiCfg.stepHeat == 5) {
    lv_imgbtn_set_src_both(buttonStep, "F:/bmp_step5_degree.bin");
  }
  else if (uiCfg.stepHeat == 10) {
    lv_imgbtn_set_src_both(buttonStep, "F:/bmp_step10_degree.bin");
  }

  if (gCfgItems.multiple_language) {
    if (uiCfg.stepHeat == 1) {
      lv_label_set_text(labelStep, preheat_menu.step_1c);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if (uiCfg.stepHeat == 5) {
      lv_label_set_text(labelStep, preheat_menu.step_5c);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if (uiCfg.stepHeat == 10) {
      lv_label_set_text(labelStep, preheat_menu.step_10c);
      lv_obj_align(labelStep, buttonStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void lv_clear_preHeat() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
