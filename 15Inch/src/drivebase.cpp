#include "vex.h"
#include "controls/drivebase.hpp"
#include "controls/pid.hpp"
#include <cmath>

using namespace vex;

void turnAuto(PID& turnPID);

int main() {
  vexcodeInit();
  DriveBase driveBase;
  PID drivePID(0.4, 0.0, 0.0, 5.0, 50.0, -100.0, 100.0, true);
  bool lastButtonAState = false;

  while (true) {
    driveBase.updateDriveMode();
    driveBase.driveControl();

    bool buttonAState = Controller1.ButtonA.pressing();
    if (buttonAState && !lastButtonAState) {
      turnAuto(drivePID);
    }
    lastButtonAState = buttonAState;
    wait(20, msec);
  }
}

void turnAuto(PID& drivePID) {
  Brain.Screen.print("Calibrating inertial sensor");
  Inertial.calibrate();
  waitUntil(!Inertial.isCalibrating());
  Brain.Screen.clearScreen();
  const double targetDegrees = 180.0;
  const double toleranceDegrees = 1.5;
  const double timeoutSeconds = 5.0;
  const int requiredSettledCycles = 5;
  int settledCycles = 0;
  timer turnTimer;
  timer loopTimer;

  leftMotorGroup.stop();
  rightMotorGroup.stop();
  Inertial.setRotation(0, rotationUnits::deg);
  drivePID.reset();
  turnTimer.reset();
  loopTimer.reset();

  while (turnTimer.time(timeUnits::sec) < timeoutSeconds) {
    double error = targetDegrees - Inertial.rotation(rotationUnits::deg);
    double dt = loopTimer.time(timeUnits::sec);
    loopTimer.reset();
    if (dt < 0.001) {
      dt = 0.02;
    }

    double output = drivePID.update(error, dt);
    if (fabs(error) <= toleranceDegrees) {
      leftMotorGroup.stop(hold);
      rightMotorGroup.stop(hold);
      settledCycles++;
      if (settledCycles >= requiredSettledCycles) {
        break;
      }
    } else {
      settledCycles = 0;
      double speed = fabs(output);
      if (speed < 8.0) {
        speed = 8.0;
      }

      leftMotorGroup.setVelocity(speed, percentUnits::pct);
      rightMotorGroup.setVelocity(speed, percentUnits::pct);

      if (output > 0.0) {
        leftMotorGroup.spin(reverse);
        rightMotorGroup.spin(forward);
      } else {
        leftMotorGroup.spin(forward);
        rightMotorGroup.spin(reverse);
      }
    }

    wait(20, msec);
  }

  leftMotorGroup.stop(hold);
  rightMotorGroup.stop(hold);
  Controller1.Screen.clearLine();
  Controller1.Screen.setCursor(1, 1);
  if (settledCycles >= requiredSettledCycles) {
    Controller1.Screen.print("Turn: ");
  } else {
    Controller1.Screen.print("Turn timeout: ");
  }
  Controller1.Screen.print(Inertial.rotation(rotationUnits::deg));
}
