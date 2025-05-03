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
};
/*
class DrivetrainQuadM {
private:
  pros::Motor _leftFront;
  pros::Motor _leftBack;
  pros::Motor _rightFront;
  pros::Motor _rightBack;
  int _gearratio;
  int _wheelbasewidth;
  int _wheeldiameter;
  int _turnVelocity = 60;
  int _driveVelocity = 60;

public:
  DrivetrainQuadM(uint8_t leftFrontPort, uint8_t leftBackPort,
                  uint8_t rightFrontPort, uint8_t rightBackPort,
                  float gearRatio, uint8_t wheelbaseWidth,
                  uint8_t wheelDiameter)
      : _leftFront(leftFrontPort), _leftBack(leftBackPort),
        _rightFront(rightFrontPort), _rightBack(rightBackPort),
        _gearratio(gearRatio), _wheelbasewidth(wheelbaseWidth),
        _wheeldiameter(wheelDiameter) {}

  void set_Velocity_Drive(int velocity) { _driveVelocity = velocity; }

  void set_Velocity_Turn(int velocity) { _turnVelocity = velocity; }

  void drive(int8_t velocity) {
    _leftFront.move(velocity);
    _leftBack.move(velocity);
    _rightFront.move(velocity);
    _rightBack.move(velocity);
  }

  void drive_For(int8_t inches) {
    float motorDegrees =
        (360 * (inches / (_wheeldiameter * M_PI))) * _gearratio;

    _leftFront.move_relative(motorDegrees, _driveVelocity);
    _leftBack.move_relative(motorDegrees, _driveVelocity);
    _rightFront.move_relative(motorDegrees, _driveVelocity);
    _rightBack.move_relative(motorDegrees, _driveVelocity);
  }

  void stop() {
    _leftFront.move(0);
    _leftBack.move(0);
    _rightFront.move(0);
    _rightBack.move(0);
  }

  void turn_Right() {
    int turnSpeed = (_turnVelocity / 2); // Adjust the speed for turning
    _leftFront.move(turnSpeed);
    _leftBack.move(turnSpeed);
    _rightFront.move(-turnSpeed);
    _rightBack.move(-turnSpeed);
  }

  void turn_Left() {
    int turnSpeed = (_turnVelocity / 2); // Adjust the speed for turning
    _leftFront.move(-turnSpeed);
    _leftBack.move(-turnSpeed);
    _rightFront.move(turnSpeed);
    _rightBack.move(turnSpeed);
  }

  void turn_For(uint8_t turnDegrees) {

    int motorDegrees =
        (_wheelbasewidth * turnDegrees) / (_wheeldiameter * M_PI);
  };
};*/