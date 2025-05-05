#include "math.h"
#include "pros/motor_group.hpp"
#include <cstdint>

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
  }

  void set_Velocity_Drive(int velocity) { _driveVelocity = velocity; }

  void set_Velocity_Turn(int velocity) { _turnVelocity = velocity; }

  void drive(int8_t velocity) {
    _left.move(velocity);
    _right.move(velocity);
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

  void turn_Pivot_Right() {
    _left.move(_turnVelocity);
    _right.move(-_turnVelocity);
  }

  void turn_Pivot_Left() {
    _left.move(-_turnVelocity);
    _right.move(_turnVelocity);
  }
  void turn_Pivot_For(int8_t turnDegrees) {

    int motorDegrees = (_wheelbasewidth * turnDegrees) / _wheeldiameter;

    _left.move_relative(motorDegrees, _turnVelocity);
    _right.move_relative(-motorDegrees, _turnVelocity);
  }
  void turn_Sweep_For(float turnDegrees, float vertical, float horizontal) {
    float theta = turnDegrees * (M_PI / 180.0);
    if (theta == 0)
      return;

    float chord = sqrt(horizontal * horizontal + vertical * vertical);
    float r = chord / (2.0f * sin(theta / 2.0f));

    float leftRadius = r - (_wheelbasewidth / 2.0f);
    float rightRadius = r + (_wheelbasewidth / 2.0f);

    float leftArc = leftRadius * theta;
    float rightArc = rightRadius * theta;

    float wheelCircumference = M_PI * _wheeldiameter;
    float leftDeg = (leftArc / wheelCircumference) * 360.0f *
                    static_cast<float>(_gearratio);
    float rightDeg = (rightArc / wheelCircumference) * 360.0f *
                     static_cast<float>(_gearratio);

    // Calculate speed ratio
    float leftSpeed, rightSpeed;
    if (leftArc > rightArc) {
      leftSpeed = _turnVelocity;
      rightSpeed = _turnVelocity * (rightArc / leftArc);
    } else {
      rightSpeed = _turnVelocity;
      leftSpeed = _turnVelocity * (leftArc / rightArc);
    }

    _left.move_relative(leftDeg, leftSpeed);
    _right.move_relative(rightDeg, rightSpeed);
  }

private:
  std::vector<std::int8_t> _ports;
  pros::MotorGroup _left;
  pros::MotorGroup _right;
  int _gearratio;
  int _wheelbasewidth;
  int _wheeldiameter;
  int _turnVelocity = 60;
  int _driveVelocity = 60;
  static pros::MotorGroup makeMotorGroup(const std::vector<int8_t> &ports) {
    return pros::MotorGroup({ports.begin(), ports.end()});
  }
};