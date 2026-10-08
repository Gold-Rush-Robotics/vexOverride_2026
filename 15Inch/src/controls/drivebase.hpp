#pragma once

#include "vex.h"

using namespace vex;

class DriveBase {
public:
  // Creates a drive controller with default state.
  DriveBase() = default;

  // Toggles between tank and arcade drive when button X is pressed.
  void updateDriveMode() {
    bool currentButtonXState = Controller1.ButtonX.pressing();

    if (currentButtonXState && !lastButtonXState) {
      isTankDrive = !isTankDrive;

      Controller1.Screen.clearLine(1);
      Controller1.Screen.setCursor(1, 1);
      if (isTankDrive) {
        Controller1.Screen.print("Mode: Tank Drive");
      } else {
        Controller1.Screen.print("Mode: Arcade Drive");
      }
    }

    lastButtonXState = currentButtonXState;
  }

  // Reads controller input and applies the active drive mode to the motors.
  void driveControl() {
    int leftSpeed = 0;
    int rightSpeed = 0;

    if (isTankDrive) {
      leftSpeed = applyDeadband(Controller1.Axis2.position());
      rightSpeed = applyDeadband(Controller1.Axis3.position());
    } else {
      int forwardInput = applyDeadband(Controller1.Axis3.position());
      int turnInput = applyDeadband(Controller1.Axis4.position());

      leftSpeed = clamp(forwardInput - turnInput);
      rightSpeed = clamp(forwardInput + turnInput);
    }

    if (leftSpeed == 0 && rightSpeed == 0) {
      myDriveTrain.stop();
    } else {
      leftMotorGroup.setVelocity(leftSpeed, percent);
      rightMotorGroup.setVelocity(rightSpeed, percent);

      leftMotorGroup.spin(forward);
      rightMotorGroup.spin(forward);
    }
  }

private:
  static const int DEADBAND = 5;

  bool isTankDrive = false;
  bool lastButtonXState = false;

  // Keeps motor values within the allowed speed range.
  int clamp(int val, int minVal = -100, int maxVal = 100) {
    if (val > maxVal) return maxVal;
    if (val < minVal) return minVal;
    return val;
  }

  // Applies a deadband to the input value to prevent small joystick movements from affecting the drive.
  int applyDeadband(int value) {
    if (abs(value) < DEADBAND) return 0;
    return value;
  }
};
