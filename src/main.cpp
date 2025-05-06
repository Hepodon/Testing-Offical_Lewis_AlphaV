#include "main.h"
#include "pros/bot.hpp"

using namespace pros;
Drivetrain Drivetrain({20}, {19}, 1, 10, 4);

Bot botton(Drivetrain, 21, 18);

void initialize() {
  delay(1000);
  botton.definePosition(0, 0, 0);
  delay(1000);
}

void disabled() {}

void competition_initialize() {}

void autonomous() {}

void opcontrol() { botton.move_To_Pos_PYTHAG(12, 15); }