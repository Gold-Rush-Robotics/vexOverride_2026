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

// Controls the drivetrain based on the controller's joystick positions
void driveControl();

//checks between arcade and tank control and returns the current drive style
bool checkDriveStyle(bool arcadeControl);

// sets the drive style based on the current drive style
void setDriveStyle(bool arcadeControl);

// gets the temperature of the left and right motors and displays it on the controller's screen
void getTemperature();


// variables
bool arcadeControl = false;

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
    
    // // Initialising axis velocities
    // int yVel = applyDeadband(Controller1.Axis3.position()); // drive Velocity
    // int xVel = applyDeadband(Controller1.Axis4.position()); // turn Velocity

    // int leftMotorSpeed = clamp(yVel + xVel); // drive + turn
    // int rightMotorSpeed = clamp(yVel - xVel); // drive - turn

    // leftMotorGroup.setVelocity(leftMotorSpeed, percent);
    // rightMotorGroup.setVelocity(rightMotorSpeed, percent);

    // // Spin both motors in the forward direction
    // leftMotorGroup.spin(forward);
    // rightMotorGroup.spin(forward);

    driveControl();
    //arcadeControl = checkDriveStyle(arcadeControl);
    //setDriveStyle(arcadeControl);
    //getTemperature();

    

    wait(25, msec);
  }
}





void driveControl() {

  //Drive Control
  if (Controller1.Axis3.position() > 5) {
    myDriveTrain.setDriveVelocity(Controller1.Axis3.position() < 100 ? Controller1.Axis3.position() : 100, percent);
    myDriveTrain.drive(forward);
  } else if (Controller1.Axis3.position() < -5) {
    myDriveTrain.setDriveVelocity(-Controller1.Axis3.position() < 100 ? -Controller1.Axis3.position() : 100, percent);
    myDriveTrain.drive(reverse);
  } else {
    myDriveTrain.stop();
  }

  // Turn control
  if(Controller1.Axis4.position() > 5) {
    myDriveTrain.setTurnVelocity(Controller1.Axis4.position() < 100 ? Controller1.Axis4.position() : 100, percent);
    myDriveTrain.turn(right);
  } else if (Controller1.Axis4.position() < -5) {
    myDriveTrain.setTurnVelocity(-Controller1.Axis4.position() < 100 ? -Controller1.Axis4.position() : 100, percent);
    myDriveTrain.turn(left);
  } else {
    myDriveTrain.stop();
  }
  
}






bool checkDriveStyle(bool arcadeControl) {
  if (Controller1.ButtonX.pressing()) {
    if(arcadeControl) {
      return false;
    }
  } else if (Controller1.ButtonX.pressing()) {
    if(!arcadeControl) {
      return true;
    }
  }

  return false;
}






void setDriveStyle(bool arcadeControl) {
  if(arcadeControl) {
    // Code for arcade control
   if (Controller1.Axis3.position() > 5) {
      myDriveTrain.setTurnVelocity(Controller1.Axis3.position() < 100 ? Controller1.Axis3.position() : 100, percent);
      myDriveTrain.drive(forward);
      myDriveTrain.turn(right);
    } else if (Controller1.Axis3.position() < -5) {
      myDriveTrain.setTurnVelocity(-Controller1.Axis3.position() < 100 ? -Controller1.Axis3.position() : 100, percent);
      myDriveTrain.drive(reverse);
      myDriveTrain.turn(right);
    } else {
      myDriveTrain.stop();
    }

    if (Controller1.Axis1.position() > 5) {
      myDriveTrain.setTurnVelocity(Controller1.Axis1.position() < 100 ? Controller1.Axis1.position() : 100, percent);
      myDriveTrain.drive(forward);
      myDriveTrain.turn(left);
    } else if (Controller1.Axis1.position() < -5) {
      myDriveTrain.setTurnVelocity(-Controller1.Axis1.position() < 100 ? -Controller1.Axis1.position() : 100, percent);
      myDriveTrain.drive(reverse);
      myDriveTrain.turn(left);
    } else {
      myDriveTrain.stop();
    }
    
  } else {
    driveControl();
  }
}





void getTemperature() {
  // Get the temperature of the left motor
  double leftMotorTemp = leftMotorGroup.temperature();
  // Get the temperature of the right motor
  double rightMotorTemp = rightMotorGroup.temperature();

  // Display the temperatures on the controller's screen
  Controller1.Screen.clearScreen();
  Controller1.Screen.setCursor(1, 1);
  Controller1.Screen.print("Left Motor Temp: %.2f C", leftMotorTemp); // Print left motor temperature to controller screen
  Controller1.Screen.setCursor(2, 1);
  Controller1.Screen.print("Right Motor Temp: %.2f C", rightMotorTemp); // Print right motor temperature to controller screen

  printf("Left Motor Temp: %.2f C\n", leftMotorTemp); // Print left motor temperature to console
  printf("Right Motor Temp: %.2f C\n", rightMotorTemp); // Print right motor temperature to console


  wait(150, msec); // Update every 150 milliseconds
  printf("\033[2J"); // Clear the console screen
}







