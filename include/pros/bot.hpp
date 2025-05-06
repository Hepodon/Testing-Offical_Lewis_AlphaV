#include "math.h"
#include "pros/drivetrain.hpp"
#include "pros/rtos.hpp"

class Bot {
public:
  Bot(Drivetrain &drivetrain, int chainPort, int dumpPort, int chainSpeed = 100)
      : _drivetrain(drivetrain), _chain(chainPort), _dump(dumpPort),
        _chainspeed(chainSpeed), _monitorTask([this] { monitorTask(); }) {}

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
    _isBusy = true;
    _drivetrain.turn_Pivot_For(angle, true);
  }

  void turn_To_Angle(int angle, bool waitForCompletion = true) {
    int delta = angle - _angle;
    turn_Pivot_For(delta, waitForCompletion);
  }

  void definePosition(int x, int y, int angle) {
    _x = x;
    _y = y;
    _angle = angle;
  }

  void move_To_Horizontal_Pos(int x, bool waitForCompletion = true) {
    if (x == _x) {
      return;
    }
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
  void move_To_Pos_PYTHAG(int x, int y) {
    int deltax = _x - x;
    int deltay = _y - y;
    float distance = sqrt(pow(deltax, 2) + pow(deltay, 2));
    float angle = atan2(deltay, deltax);
    turn_To_Angle(angle, true);
    drive_For(distance, _driveVelocity, true);
  }

  bool isBusy() const { return _isBusy; }

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
};
