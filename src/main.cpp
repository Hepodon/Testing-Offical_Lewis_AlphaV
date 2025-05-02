#include "main.h"
#include "liblvgl/display/lv_display.h"
#include "pros/drivetrain.hpp"
#include "pros/motor_group.hpp"
#include "pros/motors.h"

DrivetrainDualM drive(pros::Motor(1), pros::Motor(9));

void initialize() { lvgl_init(); }

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {}