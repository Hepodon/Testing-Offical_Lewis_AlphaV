#include "pros/drivetrain.hpp"

DrivetrainDualM::DrivetrainDualM(pros::Motor left, pros::Motor right)
    : _left(left), _right(right) {}

void DrivetrainDualM::drive_forward(int speed) {
  _left.move(speed);
  _right.move(speed);
}

void DrivetrainDualM::stop() {
  _left.brake();
  _right.brake();
}
