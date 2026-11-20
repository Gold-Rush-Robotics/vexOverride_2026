#include "vex.h"
#include "controls/drivebase.hpp"

using namespace vex;

void moveToXTarget();

int main() {
  // Initialize the VEX system and robot config before the control loop starts.
  vexcodeInit();

  DriveBase driveBase;
  bool lastButtonBState = false;

  // Main robot loop: keep manual drive active and check for the GPS test trigger.
  while (true) {
    // Toggle between tank and arcade drive based on controller input.
    driveBase.updateDriveMode();
    bool buttonBState = Controller1.ButtonB.pressing();
    if (buttonBState && !lastButtonBState) {
      moveToXTarget();
    }
    lastButtonBState = buttonBState;
    driveBase.driveControl();

    // Small delay keeps the loop responsive without starving other robot tasks.
    wait(20, msec);

    
    while(Controller1.ButtonL1.pressing()) {
      bottomArmMotorGroup.setVelocity(100, percentUnits::pct);
      bottomArmMotorGroup.spin(forward);
      topArmMotorGroup.spin(forward);
    }

    while(Controller1.ButtonL2.pressing()) {
      topArmMotorGroup.setVelocity(100, percentUnits::pct);
      bottomArmMotorGroup.spin(reverse);
      topArmMotorGroup.spin(reverse);
    }

    topArmMotorGroup.stop(hold);
    bottomArmMotorGroup.stop(hold);
  }
}

// Button B moves along the X axis to the requested position, then reports X on the Brain.
void moveToXTarget() {
  const double targetX = 700.0;
  const double startX = GPS.xPosition(mm);

  
  if (startX < targetX) {
    leftMotorGroup.setVelocity(20, percentUnits::pct);
    rightMotorGroup.setVelocity(20, percentUnits::pct);
    leftMotorGroup.spin(reverse);
    rightMotorGroup.spin(reverse);
    while (GPS.xPosition(mm) < targetX) {
      wait(20, msec);
    }
  } else if (startX > targetX) {
    leftMotorGroup.setVelocity(20, percentUnits::pct);
    rightMotorGroup.setVelocity(20, percentUnits::pct);
    leftMotorGroup.spin(forward);
    rightMotorGroup.spin(forward);
    while (GPS.xPosition(mm) > targetX) {
      wait(20, msec);
    }
  }

  leftMotorGroup.stop(coast);
  rightMotorGroup.stop(coast);

  

  const double finalX = GPS.xPosition(mm) / 1000.0;
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print("X: ");
  Brain.Screen.print(finalX);
  Brain.Screen.print(" m");
}
