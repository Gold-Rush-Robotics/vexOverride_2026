#include "vex.h"

using namespace vex;

int main() {
  vexcodeInit();
  Inertial.calibrate();
  waitUntil(!Inertial.isCalibrating());
  Inertial.setRotation(0, rotationUnits::deg);
  myDriveTrain.setStopping(brake);
  while(Inertial.rotation(rotationUnits::deg) < 90) {
     myDriveTrain.setTurnVelocity(100, percentUnits::pct);
     myDriveTrain.turnFor(right, -90, rotationUnits::deg);

     // brakes when the rotation is between 89.5 and 90 degrees
     //**testing required**
     if(Inertial.rotation(rotationUnits::deg) > 89.5 && Inertial.rotation(rotationUnits::deg) < 90.0) {
       myDriveTrain.stop(brake);
     }
    
  }

  myDriveTrain.stop();
  Controller1.Screen.print("Turn complete %.2f", Inertial.rotation(rotationUnits::deg));

  /* 
    Test 1: 97.32 degreess

  */

}
