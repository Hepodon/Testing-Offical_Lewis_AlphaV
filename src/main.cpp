#include "main.h"
#include "pros/drivetrain.hpp"

using namespace pros;

DrivetrainDualM drivetrain(1, 2, 1, 10, 4);

void initialize() { lvgl_init(); }

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
  drivetrain.drive(12);
  drivetrain.turn_For(90);
  drivetrain.stop();
  drivetrain.drive_For(12);
  drivetrain.turn_Right();
  drivetrain.turn_Left();
  drivetrain.set_Velocity_Drive(12);
  drivetrain.set_Velocity_Turn(12);
}