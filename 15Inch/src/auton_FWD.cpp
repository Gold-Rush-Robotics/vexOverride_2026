#include "vex.h"
#include "robot-config.h"
#include "controls/pid.hpp"

using namespace vex;

void moveDistancePID(double targetMm) {
  const double wheelTravelMm = 320.0;
  const double dt = 0.02;
  const double toleranceMm = 8.0;

  leftMotorGroup.setPosition(0, degrees);
  rightMotorGroup.setPosition(0, degrees);
  Brain.Screen.clearScreen();

  PID distancePID(0.125, 0.005, 0.0025, 0.0, 0.0, -60.0, 60.0, false);
  int settledCycles = 0;
  int cycles = 0;

  while (settledCycles < 5 && cycles < 500) {
    double leftDistance = leftMotorGroup.position(degrees) / 360.0 * wheelTravelMm;
    double rightDistance = rightMotorGroup.position(degrees) / 360.0 * wheelTravelMm;
    double currentDistance = (leftDistance + rightDistance) / 2.0;
    double error = targetMm - currentDistance;

    Brain.Screen.clearScreen();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.print("Distance: %.1f mm", currentDistance);

    Controller1.Screen.clearScreen();
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.print("Distance: %.1f mm", currentDistance);

    if (error <= toleranceMm && error >= -toleranceMm) {
      leftMotorGroup.stop(hold);
      rightMotorGroup.stop(hold);
      settledCycles++;
    } else {
      settledCycles = 0;
      double output = distancePID.update(error, dt);
      double speed = output < 0.0 ? -output : output;

      leftMotorGroup.setVelocity(speed, percent);
      rightMotorGroup.setVelocity(speed, percent);
      leftMotorGroup.spin(output >= 0.0 ? forward : reverse);
      rightMotorGroup.spin(output >= 0.0 ? forward : reverse);
    }

    cycles++;
    wait(20, msec);
  }

  leftMotorGroup.stop(hold);
  rightMotorGroup.stop(hold);
}

void moveForward1000mm() {
  moveDistancePID(2000.0);
}

void moveBackward1000mm() {
  moveDistancePID(-2000.0);
}

void autonomous(void) {
  moveForward1000mm();
}
