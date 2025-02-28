/**
 * Modern Vintage CNC Firmware
*/

#ifdef __PLAT_LINUX__

//#define GPIO_LOGGING // Full GPIO and Positional Logging

#include "../../inc/mvCNCConfig.h"
#include "../shared/Delay.h"
#include "hardware/IOLoggerCSV.h"
#include "hardware/Heater.h"
#include "hardware/LinearAxis.h"

#include <stdio.h>
#include <stdarg.h>
#include <thread>
#include <iostream>
#include <fstream>

extern void setup();
extern void loop();

// simple stdout / stdin implementation for fake serial port
void write_serial_thread() {
  for (;;) {
    for (std::size_t i = usb_serial.transmit_buffer.available(); i > 0; i--) {
      fputc(usb_serial.transmit_buffer.read(), stdout);
    }
    std::this_thread::yield();
  }
}

void read_serial_thread() {
  char buffer[255] = {};
  for (;;) {
    std::size_t len = _MIN(usb_serial.receive_buffer.free(), 254U);
    if (fgets(buffer, len, stdin))
      for (std::size_t i = 0; i < strlen(buffer); i++)
        usb_serial.receive_buffer.write(buffer[i]);
    std::this_thread::yield();
  }
}

void simulation_loop() {
  Heater hotend(HEATER_0_PIN, TEMP_0_PIN);
  Heater bed(HEATER_BED_PIN, TEMP_BED_PIN);
  LinearAxis x_axis(X_ENABLE_PIN, X_DIR_PIN, X_STEP_PIN, X_MIN_PIN, X_MAX_PIN);
  LinearAxis y_axis(Y_ENABLE_PIN, Y_DIR_PIN, Y_STEP_PIN, Y_MIN_PIN, Y_MAX_PIN);
  LinearAxis z_axis(Z_ENABLE_PIN, Z_DIR_PIN, Z_STEP_PIN, Z_MIN_PIN, Z_MAX_PIN);
  LinearAxis extruder0(E0_ENABLE_PIN, E0_DIR_PIN, E0_STEP_PIN, P_NC, P_NC);

  #ifdef GPIO_LOGGING
    IOLoggerCSV logger("all_gpio_log.csv");
    Gpio::attachLogger(&logger);

    std::ofstream position_log;
    position_log.open("axis_position_log.csv");

    int32_t x,y,z;
  #endif

  for (;;) {

    hotend.update();
    bed.update();

    x_axis.update();
    y_axis.update();
    z_axis.update();
    extruder0.update();

    #ifdef GPIO_LOGGING
      if (x_axis.position != x || y_axis.position != y || z_axis.position != z) {
        uint64_t update = _MAX(x_axis.last_update, y_axis.last_update, z_axis.last_update);
        position_log << update << ", " << x_axis.position << ", " << y_axis.position << ", " << z_axis.position << std::endl;
        position_log.flush();
        x = x_axis.position;
        y = y_axis.position;
        z = z_axis.position;
      }
      // flush the logger
      logger.flush();
    #endif

    std::this_thread::yield();
  }
}

int main() {
  std::thread write_serial (write_serial_thread);
  std::thread read_serial (read_serial_thread);

  #ifdef MYSERIAL1
    MYSERIAL1.begin(BAUDRATE);
    SERIAL_ECHOLNPGM("x86_64 Initialized");
    SERIAL_FLUSHTX();
  #endif

  Clock::setFrequency(F_CPU);
  Clock::setTimeMultiplier(1.0); // some testing at 10x

  HAL_timer_init();

  std::thread simulation (simulation_loop);

  DELAY_US(10000);

  setup();
  for (;;) {
    loop();
    std::this_thread::yield();
  }

  simulation.join();
  write_serial.join();
  read_serial.join();
}

#endif // __PLAT_LINUX__
