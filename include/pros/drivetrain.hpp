#include "pros/motors.hpp"
#include <cstdint>

class DrivetrainDualM {
private:
  pros::Motor _left;
  pros::Motor _right;
  int _gearratio;
  int _wheelbasewidth;
  int _wheeldiameter;

public:
  DrivetrainDualM(pros::Motor left, pros::Motor right, float gearratio,
                  uint8_t wheelbasewidth, uint8_t wheeldiameter)
      : _left(left), _right(right), _gearratio(gearratio),
        _wheelbasewidth(wheelbasewidth), _wheeldiameter(wheeldiameter) {}

  void drive(int8_t speed) {
    _left.move(speed);
    _right.move(speed);
  }

  void stop() {
    _left.move(0);
    _right.move(0);
  }

  void turn(bool right, uint8_t turnDegrees, int8_t velocity) {
    int direction = right ? 1 : -1;
    int motorDegrees = (_wheelbasewidth * turnDegrees) / _wheeldiameter;

    _left.move_relative(direction * motorDegrees, velocity);
    _right.move_relative(-direction * motorDegrees, velocity);
  }
};
