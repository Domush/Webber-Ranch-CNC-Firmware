/**
 * Modern Vintage CNC Firmware
*/

/**
 * joystick.cpp - joystick input / jogging
 */

#include "../inc/mvCNCConfigPre.h"

#if ENABLED(JOYSTICK)

#include "joystick.h"

#include "../inc/mvCNCConfig.h"  // for pins
#include "../module/planner.h"

Joystick joystick;

#if ENABLED(EXTENSIBLE_UI)
#include "../lcd/extui/ui_api.h"
#endif

#if HAS_JOY_ADC_X
xyz_float_t Joystick::x; // = { 0 }
#if ENABLED(INVERT_JOY_X)
#define JOY_X(N) (16383 - (N))
#else
#define JOY_X(N) (N)
#endif
#endif
#if HAS_JOY_ADC_Y
xyz_float_t Joystick::y; // = { 0 }
#if ENABLED(INVERT_JOY_Y)
#define JOY_Y(N) (16383 - (N))
#else
#define JOY_Y(N) (N)
#endif
#endif
#if HAS_JOY_ADC_Z
xyz_float_t Joystick::z; // = { 0 }
#if ENABLED(INVERT_JOY_Z)
#define JOY_Z(N) (16383 - (N))
#else
#define JOY_Z(N) (N)
#endif
#endif

#if ENABLED(JOYSTICK_DEBUG)
void Joystick::report() {
  SERIAL_ECHOPGM("Joystick");
#if HAS_JOY_ADC_X
  SERIAL_ECHOPGM_P(SP_X_STR, JOY_X(x));
#endif
#if HAS_JOY_ADC_Y
  SERIAL_ECHOPGM_P(SP_Y_STR, JOY_Y(y));
#endif
#if HAS_JOY_ADC_Z
  SERIAL_ECHOPGM_P(SP_Z_STR, JOY_Z(z));
#endif
  SERIAL_EOL();
}
#endif

#if HAS_JOY_ADC_X || HAS_JOY_ADC_Y || HAS_JOY_ADC_Z

void Joystick::calculate(xyz_float_t &norm_jog) {
  // Do nothing if disabled (via M458 J0 or JOYSTICK_ENABLED is not set)
  if (!enabled) return;

  auto _normalize_joy = [](float &axis_jog, const int16_t raw, const int16_t(&joy_limits)[4]) {
    if (WITHIN(raw, joy_limits[0], joy_limits[3])) {
      // within limits, check deadzone
      if (raw > joy_limits[2])
        axis_jog = (raw - joy_limits[2]) / float(joy_limits[3] - joy_limits[2]);
      else if (raw < joy_limits[1])
        axis_jog = (raw - joy_limits[1]) / float(joy_limits[1] - joy_limits[0]);  // negative value
      // Map normal to jog value via quadratic relationship
      axis_jog = SIGN(axis_jog) * sq(axis_jog);
    }
  };

#if HAS_JOY_ADC_X
  static constexpr int16_t joy_x_limits[4] = JOY_X_LIMITS;
  _normalize_joy(norm_jog.x, JOY_X(x), joy_x_limits);
#endif
#if HAS_JOY_ADC_Y
  static constexpr int16_t joy_y_limits[4] = JOY_Y_LIMITS;
  _normalize_joy(norm_jog.y, JOY_Y(y), joy_y_limits);
#endif
#if HAS_JOY_ADC_Z
  static constexpr int16_t joy_z_limits[4] = JOY_Z_LIMITS;
  _normalize_joy(norm_jog.z, JOY_Z(z), joy_z_limits);
#endif
}

#endif

void Joystick::injectJogMoves() {
  // Do nothing if disabled (via M458 W0 or WII_NUNCHUCK_ENABLED is not set)
  if (!enabled) return;
#if PIN_EXISTS(JOY_EN)
  pinMode(JOY_EN_PIN, OUTPUT);
  extDigitalWrite(JOY_EN_PIN, HIGH);
#endif

  // Recursion barrier
  static bool injecting_now; // = false;
  if (injecting_now) return;

#if ENABLED(NO_MOTION_BEFORE_HOMING)
  if (TERN0(HAS_JOY_ADC_X, axis_should_home(X_AXIS)) || TERN0(HAS_JOY_ADC_Y, axis_should_home(Y_AXIS)) || TERN0(HAS_JOY_ADC_Z, axis_should_home(Z_AXIS)))
    return;
#endif

  static constexpr int QUEUE_DEPTH = 5;                                // Insert up to this many movements
  static constexpr float target_lag = 0.25f,                           // Aim for 1/4 second lag
    seg_time = target_lag / QUEUE_DEPTH;          // 0.05 seconds, short segments inserted every 1/20th of a second
  static constexpr millis_t timer_limit_ms = millis_t(seg_time * 500); // 25 ms minimum delay between insertions

  // The planner can merge/collapse small moves, so the movement queue is unreliable to control the lag
  static millis_t next_run = 0;
  if (PENDING(millis(), next_run)) return;
  next_run = millis() + timer_limit_ms;

  // Only inject a command if the planner has fewer than 5 moves and there are no unparsed commands
  if (planner.movesplanned() >= QUEUE_DEPTH || queue.has_commands_queued())
    return;

  // Normalized jog values are 0 for no movement and -1 or +1 for as max feedrate (nonlinear relationship)
  // Jog are initialized to zero and handling input can update values but doesn't have to
  // You could use a two-axis joystick and a one-axis keypad and they might work together
  xyz_float_t norm_jog{0};

  // Use ADC values and defined limits. The active zone is normalized: -1..0 (dead) 0..1
#if HAS_JOY_ADC_X || HAS_JOY_ADC_Y || HAS_JOY_ADC_Z
  joystick.calculate(norm_jog);
#endif

  // Other non-joystick poll-based jogging could be implemented here
  // with "jogging" encapsulated as a more general class.

  TERN_(EXTENSIBLE_UI, ExtUI::_jogging_update(norm_jog));

  // norm_jog values of [-1 .. 1] maps linearly to [-feedrate .. feedrate]
  xyz_float_t move_dist{0};
  float hypot2 = 0;
  LOOP_LINEAR_AXES(i) if (norm_jog[i]) {
    move_dist[i] = seg_time * norm_jog[i] * TERN(EXTENSIBLE_UI, manual_feedrate_mm_s, planner.settings.max_feedrate_mm_s)[i];
    hypot2 += sq(move_dist[i]);
  }

  if (!UNEAR_ZERO(hypot2)) {
    current_position += move_dist;
    apply_motion_limits(current_position);
    const float length = sqrt(hypot2);
    injecting_now = true;
    planner.buffer_line(current_position, length / seg_time, active_tool, length);
    injecting_now = false;
  }
}

#endif // JOYSTICK
