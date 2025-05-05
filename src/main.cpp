#include "main.h"
#include "pros/drivetrain.hpp"

using namespace pros;

DrivetrainDualM drivetrain(1, 2);

void initialize() { lvgl_init(); }

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() { drivetrain.turn_Sweep_For(100, 10, 2); }