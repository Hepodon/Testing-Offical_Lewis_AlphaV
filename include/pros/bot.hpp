#include "math.h"
#include "pros/drivetrain.hpp"
#include "pros/rtos.hpp"

class Bot {
public:
  Bot(Drivetrain &drivetrain, int chainPort, int dumpPort, int chainSpeed = 100)
      : _drivetrain(drivetrain), _chain(chainPort), _dump(dumpPort),
        _chainspeed(chainSpeed), _monitorTask([this] { monitorTask(); }),
        _botLoopTask([this] { botLoop(); }) {}

  void set_Velocity_Drive(int velocity) { _driveVelocity = velocity; }
  void set_Velocity_Turn(int velocity) { _turnVelocity = velocity; }

  void drive(int velocity = 0) {
    if (velocity == 0)
      velocity = _driveVelocity;
    _drivetrain.drive(velocity);
  }

  void drive_For(int inches, int velocity = 0, bool waitForCompletion = true) {
    if (velocity == 0)
      velocity = _driveVelocity;
    _isBusy = true;
    _drivetrain.drive_For(inches, velocity);
    if (waitForCompletion)
      waitUntilComplete();
  }

  void turn_Pivot_For(int angle, bool waitForCompletion = true) {
    _angle += angle;
    if (waitForCompletion) {
      turnToAnglePID(_angle);
    } else {
      targetAngle = _angle;
      pidEnabled = true;
    }
  }

  void turnToAnglePID(int angleTarget) {
    pidEnabled = true;
    targetAngle = angleTarget;
    _isBusy = true;

    while (fabs(pidError) > 2.0) { // 2 tolerance
      pros::delay(20);
    }

    _drivetrain.brake();
    _drivetrain.brake();

    pidEnabled = false;
    _isBusy = false;
  }

  void definePosition(int x, int y, int angle) {
    _x = x;
    _y = y;
    _angle = angle;
    currentAngle = angle;
  }

  void move_To_Horizontal_Pos(int x, bool waitForCompletion = true) {
    if (x == _x)
      return;
    int deltax = x - _x;
    int deltaAngle = ((x > _x) ? 90 : -90) - _angle;
    turn_Pivot_For(deltaAngle, true);
    pros::delay(150);
    drive_For(fabs(deltax), 0, waitForCompletion);
    _x += deltax;
  }

  void move_To_Vertical_Pos(int y, bool waitForCompletion = true) {
    if (y == _y)
      return;
    int deltay = y - _y;
    int deltaAngle = ((y > _y) ? 0 : 180) - _angle;
    turn_Pivot_For(deltaAngle, true);
    pros::delay(150);
    drive_For(fabs(deltay), 0, waitForCompletion);
    _y += deltay;
  }

  void move_To_Pos(int x, int y, bool horizontalFirst = true) {
    if (horizontalFirst) {
      move_To_Horizontal_Pos(x, true);
      move_To_Vertical_Pos(y, true);
    } else {
      move_To_Vertical_Pos(y, true);
      move_To_Horizontal_Pos(x, true);
    }
  }

  void move_To_Pos_PYTHAG(int x, int y, bool waitForCompletion = true) {
    if (x == _x && y == _y)
      return;

    int deltax = x - _x;
    int deltay = y - _y;

    int distance = sqrt(pow(deltax, 2) + pow(deltay, 2));
    int angle = atan2(deltay, deltax) * 180 / M_PI;

    int deltaAngle = angle - _angle;
    turn_Pivot_For(deltaAngle);
    pros::delay(150);
    drive_For(distance, 0, waitForCompletion);

    _x = x;
    _y = y;
  }

  bool isBusy() const { return _isBusy; }

  // === PID-related members ===
  double totalMotorDegrees = 0;
  double currentAngle = 0;
  double targetAngle = 0;

  double kP = 0.6;
  double kI = 0.0;
  double kD = 0.05;

  double pidError = 0;
  double pidIntegral = 0;
  double pidLastError = 0;

  double lastLeft = 0;
  double lastRight = 0;

  bool pidEnabled = false;

private:
  Drivetrain &_drivetrain;
  pros::Motor _chain;
  pros::Motor _dump;
  int _x = 0;
  int _y = 0;
  int _angle = 0;
  int _chainspeed = 100;
  int _turnVelocity = 35;
  int _driveVelocity = 55;
  bool _isBusy = false;

  pros::Task _monitorTask;
  pros::Task _botLoopTask;

  void monitorTask() {
    while (true) {
      if (!_drivetrain.isBusy())
        _isBusy = false;
      pros::delay(10);
    }
  }

  void waitUntilComplete() {
    while (_isBusy)
      pros::delay(10);
  }

  void updateRotation() {
    double left = _drivetrain.get_Position_Left();
    double right = _drivetrain.get_Position_Right();

    double deltaLeft = left - lastLeft;
    double deltaRight = right - lastRight;

    lastLeft = left;
    lastRight = right;

    double leftDistance = (deltaLeft / 360.0) *
                          (M_PI * _drivetrain.get_wheelDiameter()) *
                          _drivetrain.get_gearRatio();
    double rightDistance = (deltaRight / 360.0) *
                           (M_PI * _drivetrain.get_wheelDiameter()) *
                           _drivetrain.get_gearRatio();

    double deltaTheta =
        (rightDistance - leftDistance) / _drivetrain.get_wheelBaseWidth();
    currentAngle += deltaTheta * (180.0 / M_PI);

    if (currentAngle >= 360)
      currentAngle -= 360;
    if (currentAngle < 0)
      currentAngle += 360;
  }

  void updateAnglePID() {
    pidError = targetAngle - currentAngle;

    if (pidError > 180)
      pidError -= 360;
    if (pidError < -180)
      pidError += 360;

    pidIntegral += pidError;
    double derivative = pidError - pidLastError;
    pidLastError = pidError;

    double output = kP * pidError + kI * pidIntegral + kD * derivative;
    output = std::clamp(output, -100.0, 100.0);

    _drivetrain.left_Drive(-output);
    _drivetrain.right_Drive(output);
  }

  int botLoop() {
    while (true) {
      updateRotation();
      if (pidEnabled && !_isBusy)
        updateAnglePID();
      pros::delay(20);
    }
    return 0;
  }
};
