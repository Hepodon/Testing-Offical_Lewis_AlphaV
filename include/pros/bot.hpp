#include "math.h"
#include "pros/abstract_motor.hpp"
#include "pros/drivetrain.hpp"
#include "pros/motor_group.hpp"
#include <cstdint>

class Bot {
public:
  Bot(Drivetrain &drivetrain, int chainPort, int dumpPort, int chainSpeed = 100)
      : _drivetrain(drivetrain), _chain(chainPort), _dump(dumpPort),
        _chainspeed(chainSpeed) {}
  void set_Velocity_Drive(int velocity) { _driveVelocity = velocity; }

  void set_Velocity_Turn(int velocity) { _turnVelocity = velocity; }
  void drive(int velocity = 0) {
    if (velocity == 0)
      velocity = _driveVelocity;
    _drivetrain.drive(_driveVelocity);
  }

private:
  Drivetrain &_drivetrain;
  pros::Motor _chain;
  pros::Motor _dump;
  int _chainspeed = 100;
  int _turnVelocity = 40;
  int _driveVelocity = 60;
};