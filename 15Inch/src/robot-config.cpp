#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 brain screen
brain Brain;

// VEXcode device constructors

// Clawbot drive base ports for moters
controller Controller1 = controller(primary);
motor leftMotor1 = motor(PORT5, ratio6_1, false);
motor leftMotor2 = motor(PORT1, ratio6_1, false);

motor rightMotor1 = motor(PORT2, ratio6_1, true);
motor rightMotor2 = motor(PORT3, ratio6_1, true);

// 24in drive base ports
// motor leftMotor1 = motor(PORT5, ratio6_1, false);
// motor leftMotor2 = motor(PORT3, ratio6_1, true);
// motor leftMotor3 = motor(PORT4, ratio6_1, false);

// motor rightMotor1 = motor(PORT11, ratio6_1, true);
// motor rightMotor2 = motor(PORT12, ratio6_1, false);
// motor rightMotor3 = motor(PORT13, ratio6_1, true);


//VEX motor groups
motor_group leftMotorGroup = motor_group(leftMotor1, leftMotor2);
motor_group rightMotorGroup = motor_group(rightMotor1, rightMotor2);
drivetrain DriveTrain = drivetrain(leftMotorGroup, rightMotorGroup, 320, 320, 120, mm, 1);

//VEX sensors
inertial Inertial = inertial(PORT20);

// VEXcode generated functions

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // nothing to initialize
}