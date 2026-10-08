#include "controls/drivebase.hpp"

int main() {
  vexcodeInit();
  myDriveTrain.setStopping(coast);

  DriveBase driveBase;

  while (true) {
    driveBase.updateDriveMode();
    driveBase.driveControl();
    wait(20, msec);
  }
}