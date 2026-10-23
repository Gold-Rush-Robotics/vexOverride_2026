#include "vex.h"
#include "controls/drivebase.hpp"

using namespace vex;

void turnAuto();

int main() {
  vexcodeInit();
  DriveBase driveBase;

  while(true) {
    driveBase.updateDriveMode();
    driveBase.driveControl();

    if(Controller1.ButtonA.pressing()) {
      turnAuto();
    }
  }
  /* 
    Test 1: 97.32 degreess

  */

}

void turnAuto() {
  
  Inertial.calibrate();
  Brain.Screen.print("Calibrating Inertial Sensor");
  waitUntil(!Inertial.isCalibrating());
  Brain.Screen.clearScreen();
  
  Inertial.setRotation(0, rotationUnits::deg);

  leftMotorGroup.stop();
  rightMotorGroup.stop();

  while(Inertial.rotation(rotationUnits::deg) < 180 ) {
     leftMotorGroup.spin(reverse, 100, percentUnits::pct);
     rightMotorGroup.spin(forward, 100, percentUnits::pct);
  }
  leftMotorGroup.stop(hold);
  rightMotorGroup.stop(hold);
  Controller1.Screen.print(Inertial.rotation(rotationUnits::deg));
}
