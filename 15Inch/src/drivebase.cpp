#include "vex.h"

using namespace vex;

const int DEADBAND = 5;

// Mode toggle tracking
bool isTankDrive = false; 
bool lastButtonXState = false;

int clamp(int val, int minVal = -100, int maxVal = 100) {
  if (val > maxVal) return maxVal;
  if (val < minVal) return minVal;
  return val;
}

int applyDeadband(int value) {
  if (abs(value) < DEADBAND) return 0;
  return value;
}

void updateDriveMode() {
  bool currentButtonXState = Controller1.ButtonX.pressing();

  // Trigger toggle only on the rising edge (transition from unpressed to pressed)
  if (currentButtonXState && !lastButtonXState) {
    isTankDrive = !isTankDrive;

    // Update the controller screen to show the current drive mode
    Controller1.Screen.clearLine(1);
    Controller1.Screen.setCursor(1, 1);
    if (isTankDrive) {
      Controller1.Screen.print("Mode: Tank Drive");
    } else {
      Controller1.Screen.print("Mode: Arcade Drive");
    }
  }

  // Update history state for edge-detection
  lastButtonXState = currentButtonXState;
}

void driveControl() {
  int leftSpeed = 0;
  int rightSpeed = 0;

  if (isTankDrive) {
    // --- TANK DRIVE ---
    // Axis 2 = Left side speed, Axis 3 = Right side speed
    leftSpeed  = applyDeadband(Controller1.Axis2.position());
    rightSpeed = applyDeadband(Controller1.Axis3.position());
  } else {
    // --- ARCADE DRIVE ---
    // Axis 3 = Throttle (Forward/Back), Axis 4 = Steering (Left/Right)
    int forwardInput = applyDeadband(Controller1.Axis3.position());
    int turnInput    = applyDeadband(Controller1.Axis4.position());

    leftSpeed  = clamp(forwardInput - turnInput);
    rightSpeed = clamp(forwardInput + turnInput);
  }

  // Output to motors
  if (leftSpeed == 0 && rightSpeed == 0) {
    myDriveTrain.stop();
  } else {
    leftMotorGroup.setVelocity(leftSpeed, percent);
    rightMotorGroup.setVelocity(rightSpeed, percent);

    leftMotorGroup.spin(forward);
    rightMotorGroup.spin(forward);
  }
}

int main() {
  vexcodeInit();
  myDriveTrain.setStopping(coast);

  while (true) {
    updateDriveMode(); // Check for button press
    driveControl();    // Drive robot based on active mode (tank or arcade)
    wait(20, msec);    
  }
}