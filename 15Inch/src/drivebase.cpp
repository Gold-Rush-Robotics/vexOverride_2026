#include "vex.h"

using namespace vex;

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

int main() {
  // Initializing Robot Configuration
  vexcodeInit();

  // // Code for arcade control
  // while (true) {
  //   // Get the velocity percentage of the left motor. (Axis3)
  //   int leftMotorSpeed = Controller1.Axis3.position();
  //   // Get the velocity percentage of the right motor. (Axis2)
  //   int rightMotorSpeed = Controller1.Axis2.position();

  //   deadzone(leftMotorSpeed, rightMotorSpeed);

  //   // Spin both motors in the forward direction.
  //   leftMotorGroup.spin(forward);
  //   rightMotorGroup.spin(forward);

  //   wait(25, msec);
  // }

  // Code for single joystick control
  while(true) {
    
    // Initialising axis velocities
    int yVel = applyDeadband(Controller1.Axis3.position()); // drive Velocity
    int xVel = applyDeadband(Controller1.Axis4.position()); // turn Velocity

    int leftMotorSpeed = clamp(yVel + xVel); // drive + turn
    int rightMotorSpeed = clamp(yVel - xVel); // drive - turn

    leftMotorGroup.setVelocity(leftMotorSpeed, percent);
    rightMotorGroup.setVelocity(rightMotorSpeed, percent);

    // Spin both motors in the forward direction
    leftMotorGroup.spin(forward);
    rightMotorGroup.spin(forward);

    wait(25, msec);
  }
}
