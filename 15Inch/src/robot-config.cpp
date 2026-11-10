#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

// Calling VEXcode sensor constructors
inertial Inertial = inertial(PORT10);
gps GPS = gps(PORT1);

controller Controller1 = controller(primary);
motor leftMotor1 = motor(PORT5, ratio6_1, false);
motor leftMotor2 = motor(PORT3, ratio6_1, true);
motor leftMotor3 = motor(PORT4, ratio6_1, false);
motor rightMotor1 = motor(PORT11, ratio6_1, true);
motor rightMotor2 = motor(PORT12, ratio6_1, false);
motor rightMotor3 = motor(PORT13, ratio6_1, true);

motor bottomArmMotor1 = motor(PORT1, ratio18_1, false); 
motor bottomArmMotor2 = motor(PORT11, ratio18_1, true);

motor topArmMotor1 = motor(PORT3, ratio18_1, true);
motor topArmMotor2 = motor(PORT12, ratio18_1, false);

//VEX motor groups
motor_group leftMotorGroup = motor_group(leftMotor1, leftMotor2, leftMotor3);
motor_group rightMotorGroup = motor_group(rightMotor1, rightMotor2, rightMotor3);
motor_group bottomArmMotorGroup = motor_group(bottomArmMotor1, bottomArmMotor2);
motor_group topArmMotorGroup = motor_group(topArmMotor1, topArmMotor2);

drivetrain myDriveTrain = drivetrain(leftMotorGroup, rightMotorGroup, 320, 320, 130, mm, 1.0);


// VEXcode generated functions

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // nothing to initialize
}