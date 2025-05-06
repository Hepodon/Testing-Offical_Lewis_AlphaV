#include "main.h"
#include "pros/bot.hpp"

using namespace pros;
Drivetrain Drivetrain({20}, {-19}, 1, 10, 4);

Bot botton(Drivetrain, 21, 18);

void initialize() {
  delay(1000);
  botton.definePosition(0, 0, 0);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() {
  botton.move_To_Horizontal_Pos(-12, true);
  botton.move_To_Horizontal_Pos(12, true);
  // botton.move_To_Horizontal_Pos(12, true);
  // // Drivetrain.turn_Pivot_For(90);
  // botton.test(15);
  // Drivetrain.drive_For(12);
}