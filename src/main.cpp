#include "main.h"
#include "lewis/create.hpp"
#include "liblvgl/display/lv_display.h"
#include "pros/motor_group.hpp"
#include "pros/motors.h"

lewis::Drivetrain driver({12, 13}, v5::MotorGears::green,
                         v5::MotorEncoderUnits::degrees);
pros::MotorGroup

void initialize() { lvgl_init(); }

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
  create.box("name", 10, 10, 100, 100);
  create.triangle("Name", 1, 1, 1, 1, 1, 1);
  driver
}