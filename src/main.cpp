#include "main.h"
#include "liblvgl/display/lv_display.h"
#include "pros/drivetrain.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.h"

using namespace pros;

DrivetrainDualM drivetrain(Motor(1), Motor(20), 1, 10, 4);

void initialize() { lvgl_init(); }

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
  drivetrain.drive(12);
  drivetrain.turn(true, 90, 12);
  drivetrain.stop();
}