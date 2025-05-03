#include "math.h"
#include "pros/motors.hpp"
#include <cstdint>

class DrivetrainDualM {
private:
  pros::Motor _left;
  pros::Motor _right;
  int _gearratio;
  int _wheelbasewidth;
  int _wheeldiameter;
  int _turnVelocity = 60;
  int _driveVelocity = 60;

public:
  DrivetrainDualM(uint8_t leftPort, uint8_t rightPort, float gearRatio,
                  uint8_t wheelbaseWidth, uint8_t wheelDiameter)
      : _left(leftPort), _right(rightPort), _gearratio(gearRatio),
        _wheelbasewidth(wheelbaseWidth), _wheeldiameter(wheelDiameter) {}

  void set_Velocity_Drive(int velocity) { _driveVelocity = velocity; }

  void set_Velocity_Turn(int velocity) { _turnVelocity = velocity; }

  void drive(int8_t speed) {
    _left.move(speed);
    _right.move(speed);
  }

  void drive_For(int8_t inches) {
    float motorDegrees =
        (360 * (inches / (_wheeldiameter * M_PI))) * _gearratio;

    _left.move_relative(motorDegrees, _driveVelocity);
    _left.move_relative(motorDegrees, _driveVelocity);
  }

  void stop() {
    _left.move(0);
    _right.move(0);
  }

  void turn_Right() {
    _left.move(_turnVelocity);
    _right.move(-_turnVelocity);
  }

  void turn_Left() {
    _left.move(-_turnVelocity);
    _right.move(_turnVelocity);
  }
  void turn_For(uint8_t turnDegrees) {

    float motorDegrees = (_wheelbasewidth * turnDegrees) / _wheeldiameter;

    _left.move_relative(motorDegrees, _turnVelocity);
    _right.move_relative(-motorDegrees, _turnVelocity);
  }
};
