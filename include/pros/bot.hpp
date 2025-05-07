#include "math.h"
#include "pros/drivetrain.hpp"
#include "pros/rtos.hpp"

class Bot {
public:
  Bot(Drivetrain &drivetrain, int chainPort, int dumpPort, int chainSpeed = 100)
      : _drivetrain(drivetrain), _chain(chainPort), _dump(dumpPort),
        _chainspeed(chainSpeed), _monitorTask(monitortaskWrapper, this),
        _dumpTask(dumpTaskWrapper, this), _botTask(botLoopWrapper, this) {}

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

  void turnToAnglePID(int angleTarget) {
    pidEnabled = true;
    targetAngle = angleTarget;
    double error = targetAngle, derivative = 0, integral = 0, prevError = error;
    _isBusy = true;

    while (true) {
      double input = currentAngle;

      error = targetAngle - input;
      derivative = error - prevError;
      integral += error;

      double output = error * kP + integral * kI + derivative * kD;

      if (fabs(error) < 1) {
        _drivetrain.brake();
        _isBusy = false;
        pidEnabled = false;
        break;
      }

      _drivetrain.left_Drive(output);
      _drivetrain.right_Drive(-output);

      prevError = error;
      pros::delay(10);
    }
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
    int deltaAngle = ((x > _x) ? 90 : -90);
    turnToAnglePID(deltaAngle);
    pros::delay(150);
    drive_For(fabs(deltax), 0, waitForCompletion);
    _x += deltax;
  }

  void move_To_Vertical_Pos(int y, bool waitForCompletion = true) {
    if (y == _y)
      return;
    int deltay = y - _y;
    int deltaAngle = ((y > _y) ? 0 : 180);
    turnToAnglePID(deltaAngle);
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
    turnToAnglePID(angle);

    pros::delay(150);
    drive_For(distance, 0, waitForCompletion);

    _x = x;
    _y = y;
  }

  void chain_Disable() { _chain.brake(); }
  void chain_Active(bool forwards, int speed = 0) {
    speed = fabs(speed);
    speed = (speed == 0 ? _chainspeed : speed);
    if (forwards) {
      _chain.move(speed);
    } else {
      _chain.move(-speed);
    }
  }

  void dump(bool active = false) { _dumpActive = active; }

  bool isBusy() const { return _isBusy; }

  // === PID-related members ===
  double totalMotorDegrees = 0;
  double currentAngle = 0;
  double targetAngle = 0;

  double kP = 0.6, kI = 0, kD = 0.05;
  double input, output;

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
  bool _dumpActive = false;
  pros::Task _monitorTask;
  pros::Task _dumpTask;
  pros::Task _botTask;

  void dumpTask() {
    while (true) {
      if (_dumpActive) {
        _dump.move(-72);
        pros::delay(1500);
        _dumpActive = false;
      } else {
        _dump.move(20);
      }
      pros::delay(50);
    }
  }

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

  static void botLoopWrapper(void *ptr) { static_cast<Bot *>(ptr)->botLoop(); }
  static void dumpTaskWrapper(void *ptr) {
    static_cast<Bot *>(ptr)->dumpTask();
  }
  static void monitortaskWrapper(void *ptr) {
    static_cast<Bot *>(ptr)->monitorTask();
  }

  void botLoop() {
    while (true) {
      double left = _drivetrain.get_Position_Left();
      double right = _drivetrain.get_Position_Right();

      double deltaLeft = left - lastLeft;
      double deltaRight = right - lastRight;
      currentAngle = fmod(currentAngle, 360.0);
      if (currentAngle < 0)
        currentAngle += 360.0;

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
      pros::delay(20);
    }
  }
};
