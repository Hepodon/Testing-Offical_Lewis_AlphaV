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
    _drivetrain.turn_Pivot_For(angle);
    if (waitForCompletion)
      waitUntilComplete();
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

  void move_To_Vorizontal_Pos(int x, bool waitForCompletion = true) {
    int deltax = _x - x;
    if (deltax > _x) {
      int deltaAngle = 90 - _angle;
      turn_Pivot_For(deltaAngle, true);
      drive_For(deltax, 0, waitForCompletion);
    }
    if (deltax < _x) {
      int deltaAngle = -90 - _angle;
      turn_Pivot_For(deltaAngle, true);
      drive_For(deltax, _driveVelocity, waitForCompletion);
    }
  }
  void move_To_Vertical_Pos(int y, bool waitForCompletion = true) {
    int deltay = _y - y;
    if (deltay > _y) {
      int deltaAngle = 0 - _angle;
      turn_Pivot_For(deltaAngle, true);
      drive_For(deltay, 0, waitForCompletion);
    }
    if (deltay < _y) {
      int deltaAngle = 180 - _angle;
      turn_Pivot_For(deltaAngle, true);
      drive_For(deltay, _driveVelocity, waitForCompletion);
    }
  }
  void move_To_Pos(int x, int y, bool horizontalFirst = true) {
    if (horizontalFirst) {
      move_To_Vorizontal_Pos(x, true);
      move_To_Vertical_Pos(y, true);
    } else {
      move_To_Vertical_Pos(y, true);
      move_To_Vorizontal_Pos(x, true);
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
  int _turnVelocity = 40;
  int _driveVelocity = 60;
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
