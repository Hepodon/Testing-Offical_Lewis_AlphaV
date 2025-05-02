#pragma once
#include "pros/motors.hpp"

class DrivetrainDualM {
public:
  DrivetrainDualM(pros::Motor left, pros::Motor right);

  void drive_forward(int speed);
  void stop();

private:
  pros::Motor _left;
  pros::Motor _right;
};
