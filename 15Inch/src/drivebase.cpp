/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       VEX                                                       */
/*    Created:      Wed Sep 25 2019                                           */
/*    Description:  Tank Drive                                                */
/*    This sample allows you to control the V5 Clawbot using the both         */
/*    joystick. Adjust the deadband value for more accurate movements.        */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// Robot Configuration:
// [Name]               [Type]        [Port(s)]
// Controller1          controller
// LeftMotor            motor         1
// RightMotor           motor         10
// ClawMotor            motor         3
// ArmMotor             motor         8
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

void autonomous(void);
void moveForward1000mm();
void moveBackward1000mm();
void moveDistancePID(double targetMm);

competition Competition;

/*
  Keeps motor velocity in a valid range
*/
int clamp(int motorSpeed) {
  
  return std::max(-100, std::min(motorSpeed, 100));
}

// Prevents tiny joystick noise from causing unwanted movement.
int applyDeadband(int speed, int deadband = 5) {
  if (abs(speed) < deadband) {
    return 0;
  }
  return speed;
}

void usercontrol(void) {
  while(true) {
    
    int yVel = applyDeadband(Controller1.Axis3.position()); // drive Velocity
    int xVel = applyDeadband(Controller1.Axis4.position()); // turn Velocity

    int leftMotorSpeed = clamp(yVel + xVel); // drive + turn
    int rightMotorSpeed = clamp(yVel - xVel); // drive - turn

    leftMotorGroup.setVelocity(leftMotorSpeed, percent);
    rightMotorGroup.setVelocity(rightMotorSpeed, percent);

    leftMotorGroup.spin(forward);
    rightMotorGroup.spin(forward);

    if (Controller1.ButtonX.pressing()) {
      DriveTrain.setDriveVelocity(50, percent);
      moveForward1000mm();
      while (Controller1.ButtonX.pressing()) {
        wait(20, msec);
      }
    } else if (Controller1.ButtonB.pressing()) {
      DriveTrain.setDriveVelocity(50, percent);
      moveBackward1000mm();
      while (Controller1.ButtonB.pressing()) {
        wait(20, msec);
      }
    }

    if(Controller1.ButtonA.pressing()) {
      leftMotorGroup.setVelocity(100, percent);
      rightMotorGroup.setVelocity(100, percent);
      leftMotorGroup.spin(forward);
      rightMotorGroup.spin(forward);
      wait(10000, msec);
      leftMotorGroup.stop();
      rightMotorGroup.stop();
    }

    if(Controller1.ButtonY.pressing()) {
      leftMotorGroup.stop();
      rightMotorGroup.stop();
      wait(20, msec);
    }


    wait(25, msec);
  }
}

int main() {
  vexcodeInit();
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  while(true) {
    wait(100, msec);
  }
}
