#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor leftMotor1 = motor(PORT1, ratio6_1, true);
motor leftMotor2 = motor(PORT2, ratio6_1, true);
motor leftMotor3 = motor(PORT3, ratio6_1, true);
motor leftMotor4 = motor(PORT4, ratio6_1, true);
motor leftMotor5 = motor(PORT5, ratio6_1, true);
motor rightMotor1 = motor(PORT20, ratio6_1, false);
motor rightMotor2 = motor(PORT19, ratio6_1, false);
motor rightMotor3 = motor(PORT18, ratio6_1, false);
motor rightMotor4 = motor(PORT17, ratio6_1, false);
motor rightMotor5 = motor(PORT16, ratio6_1, false);

//VEX motor groups
motor_group leftMotorGroup = motor_group(leftMotor1, leftMotor2, leftMotor3, leftMotor4, leftMotor5);
motor_group rightMotorGroup = motor_group(rightMotor1, rightMotor2, rightMotor3, rightMotor4, rightMotor5);

// VEXcode generated functions

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // nothing to initialize
}