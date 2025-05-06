#include "math.h"
#include "pros/abstract_motor.hpp"
#include "pros/motor_group.hpp"
#include "pros/rtos.hpp"
#include <cstdint>
#include <vector>

class Drivetrain {

public:
  Drivetrain(std::vector<int8_t> leftPorts, std::vector<int8_t> rightPorts,
             float gearRatio = 1, uint8_t wheelbaseWidth = 10,
             uint8_t wheelDiameter = 4)
      : _left(makeMotorGroup(leftPorts)), _right(makeMotorGroup(rightPorts)),
        _gearratio(gearRatio), _wheelbasewidth(wheelbaseWidth),
        _wheeldiameter(wheelDiameter) {

    _left.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    _right.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);
    _left.set_encoder_units(pros::MotorEncoderUnits::degrees);
    _right.set_encoder_units(pros::MotorEncoderUnits::degrees);

    pros::Task([this]() { this->monitorTask(); });
  }

  void set_Velocity_Drive(int velocity) { _driveVelocity = velocity; }
  void set_Velocity_Turn(int velocity) { _turnVelocity = velocity; }

  void drive(int velocity) {
    _left.move(velocity);
    _right.move(velocity);
  }

  int get_wheelDiameter() { return _wheeldiameter; }
  int get_gearRatio() { return _gearratio; }
  int get_wheelBaseWidth() { return _wheelbasewidth; }

  void drive_For(int inches, int velocity = 0, bool waitForCompletion = true) {
    if (velocity == 0)
      velocity = _driveVelocity;

    float motorDegrees =
        (360 * (inches / (_wheeldiameter * M_PI))) * _gearratio;
    _driveTarget = motorDegrees;
    _isDriving = true;
    _leftStart = _left.get_position();
    _rightStart = _right.get_position();

    _left.move_relative(motorDegrees, velocity);
    _right.move_relative(motorDegrees, velocity);

    if (waitForCompletion) {
      while (_isDriving)
        pros::delay(10);
    }
  }

  void stop() {
    _left.move(0);
    _right.move(0);
    _isDriving = false;
    _isTurning = false;
  }

  void turn_Pivot_Right() {
    _left.move(_turnVelocity);
    _right.move(-_turnVelocity);
  }

  void turn_Pivot_Left() {
    _left.move(-_turnVelocity);
    _right.move(_turnVelocity);
  }

  void turn_Pivot_For(int turnDegrees, bool waitForCompletion = true) {
    float motorDegrees = (_wheelbasewidth * turnDegrees) / _wheeldiameter;

    _leftStart = _left.get_position();
    _rightStart = _right.get_position();
    _driveTarget = motorDegrees;
    _isTurning = true;

    _left.move_relative(motorDegrees, _turnVelocity);
    _right.move_relative(-motorDegrees, -_turnVelocity);

    if (waitForCompletion) {
      while (_isTurning)
        pros::delay(10);
    }
  }

  bool isBusy() const { return _isDriving || _isTurning; }

private:
  pros::MotorGroup _left;
  pros::MotorGroup _right;
  float _gearratio;
  float _wheelbasewidth;
  float _wheeldiameter;
  int _turnVelocity = 40;
  int _driveVelocity = 60;

  bool _isDriving = false;
  bool _isTurning = false;
  float _driveTarget = 0;
  float _leftStart = 0;
  float _rightStart = 0;

  void monitorTask() {
    while (true) {
      if (_isDriving || _isTurning) {
        float leftDelta = std::abs(_left.get_position() - _leftStart);
        float rightDelta = std::abs(_right.get_position() - _rightStart);
        if (leftDelta >= std::abs(_driveTarget) * 0.98 &&
            rightDelta >= std::abs(_driveTarget) * 0.98) {
          stop();
        }
      }
      pros::delay(10);
    }
  }

  static pros::MotorGroup makeMotorGroup(const std::vector<int8_t> &ports) {
    return pros::MotorGroup({ports.begin(), ports.end()});
  }
};
