#include "main.h"
#include "pros/drivetrain.hpp"

using namespace pros;
Drivetrain Drivetrain({20}, {19}, 1, 10, 4);

void initialize() { lvgl_init(); }

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
  delay(1000);
  Drivetrain.turn_Pivot_For(120);
}