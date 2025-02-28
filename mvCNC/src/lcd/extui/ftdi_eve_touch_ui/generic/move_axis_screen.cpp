/************************
 * move_axis_screen.cpp *
 ************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"
#include "../screens.h"
#include "../screen_data.h"

#ifdef FTDI_MOVE_AXIS_SCREEN

using namespace FTDI;
using namespace ExtUI;

constexpr static MoveAxisScreenData &mydata = screen_data.MoveAxisScreen;

void BaseMoveAxisScreen::onEntry() {
  // Since mvCNC keeps only one absolute position for all the extruders,
  // we have to keep track of the relative motion of individual extruders
  // ourselves. The relative distances are reset to zero whenever this
  // screen is entered.

  LOOP_L_N(i, ExtUI::extruderCount) {
    mydata.e_rel[i] = 0;
  }
  BaseNumericAdjustmentScreen::onEntry();
}

void MoveAxisScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.precision(1);
  w.units(GET_TEXT_F(MSG_UNITS_MM));
  w.heading(                           GET_TEXT_F(MSG_MOVE_AXIS));
  w.home_buttons(20);
  w.color(Theme::x_axis).adjuster(  2, GET_TEXT_F(MSG_AXIS_X),  getAxisPosition_mm(X), canMove(X));
  w.color(Theme::y_axis).adjuster(  4, GET_TEXT_F(MSG_AXIS_Y),  getAxisPosition_mm(Y), canMove(Y));
  w.color(Theme::z_axis).adjuster(  6, GET_TEXT_F(MSG_AXIS_Z),  getAxisPosition_mm(Z), canMove(Z));

  w.color(Theme::e_axis);
  #if EXTRUDERS == 1
    w.adjuster(  8, GET_TEXT_F(MSG_AXIS_E),  mydata.e_rel[0], canMove(E0));
  #elif HAS_MULTI_EXTRUDER
    w.adjuster(  8, GET_TEXT_F(MSG_AXIS_E1), mydata.e_rel[0], canMove(E0));
    w.adjuster( 10, GET_TEXT_F(MSG_AXIS_E2), mydata.e_rel[1], canMove(E1));
    #if EXTRUDERS > 2
      w.adjuster( 12, GET_TEXT_F(MSG_AXIS_E3), mydata.e_rel[2], canMove(E2));
    #endif
    #if EXTRUDERS > 3
      w.adjuster( 14, GET_TEXT_F(MSG_AXIS_E4), mydata.e_rel[3], canMove(E3));
    #endif
  #endif
  #if Z_HOME_TO_MIN
    w.button(24, GET_TEXT_F(MSG_MOVE_Z_TO_TOP), !axis_should_home(Z_AXIS));
  #endif
  w.increments();
}

bool BaseMoveAxisScreen::onTouchHeld(uint8_t tag) {
  #define UI_INCREMENT_AXIS(axis) setManualFeedrate(axis, increment); UI_INCREMENT(AxisPosition_mm, axis);
  #define UI_DECREMENT_AXIS(axis) setManualFeedrate(axis, increment); UI_DECREMENT(AxisPosition_mm, axis);
  const float increment = getIncrement();
  switch (tag) {
    case  2: UI_DECREMENT_AXIS(X); break;
    case  3: UI_INCREMENT_AXIS(X); break;
    case  4: UI_DECREMENT_AXIS(Y); break;
    case  5: UI_INCREMENT_AXIS(Y); break;
    case  6: UI_DECREMENT_AXIS(Z); break;
    case  7: UI_INCREMENT_AXIS(Z); break;
    // For extruders, also update relative distances.
    case  8: UI_DECREMENT_AXIS(E0); mydata.e_rel[0] -= increment; break;
    case  9: UI_INCREMENT_AXIS(E0); mydata.e_rel[0] += increment; break;
    #if HAS_MULTI_EXTRUDER
    case 10: UI_DECREMENT_AXIS(E1); mydata.e_rel[1] -= increment; break;
    case 11: UI_INCREMENT_AXIS(E1); mydata.e_rel[1] += increment; break;
    #endif
    #if EXTRUDERS > 2
    case 12: UI_DECREMENT_AXIS(E2); mydata.e_rel[2] -= increment; break;
    case 13: UI_INCREMENT_AXIS(E2); mydata.e_rel[2] += increment; break;
    #endif
    #if EXTRUDERS > 3
    case 14: UI_DECREMENT_AXIS(E3); mydata.e_rel[3] -= increment; break;
    case 15: UI_INCREMENT_AXIS(E3); mydata.e_rel[3] += increment; break;
    #endif
    case 20: SpinnerDialogBox::enqueueAndWait(F("G28X")); break;
    case 21: SpinnerDialogBox::enqueueAndWait(F("G28Y")); break;
    case 22: SpinnerDialogBox::enqueueAndWait(F("G28Z")); break;
    case 23: SpinnerDialogBox::enqueueAndWait(F("G28")); break;
    case 24: raiseZtoTop(); break;
    default:
      return false;
  }
  #undef UI_DECREMENT_AXIS
  #undef UI_INCREMENT_AXIS
  return true;
}

void BaseMoveAxisScreen::raiseZtoTop() {
  constexpr xyze_feedrate_t homing_feedrate = HOMING_FEEDRATE_MM_M;
  setAxisPosition_mm(Z_MAX_POS - 5, Z, homing_feedrate[Z_AXIS]);
}

float BaseMoveAxisScreen::getManualFeedrate(uint8_t axis, float increment_mm) {
  // Compute feedrate so that the tool lags the adjuster when it is
  // being held down, this allows enough margin for the planner to
  // connect segments and even out the motion.
  constexpr xyze_feedrate_t max_manual_feedrate = MANUAL_FEEDRATE;
  return min(max_manual_feedrate[axis] / 60.0f, ABS(increment_mm * (TOUCH_REPEATS_PER_SECOND) * 0.80f));
}

void BaseMoveAxisScreen::setManualFeedrate(ExtUI::axis_t axis, float increment_mm) {
  ExtUI::setFeedrate_mm_s(getManualFeedrate(X_AXIS + (axis - ExtUI::X), increment_mm));
}

void BaseMoveAxisScreen::setManualFeedrate(ExtUI::extruder_t, float increment_mm) {
  ExtUI::setFeedrate_mm_s(getManualFeedrate(E_AXIS, increment_mm));
}

void MoveAxisScreen::onIdle() {
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

#endif // FTDI_MOVE_AXIS_SCREEN
