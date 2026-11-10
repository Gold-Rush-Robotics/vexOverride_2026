using namespace vex;

extern brain Brain;

// VEXcode sensors
extern inertial Inertial;
extern gps GPS;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */

extern controller Controller1;
extern motor leftMotor1;
extern motor leftMotor2;
extern motor leftMotor3;
extern motor leftMotor4;
extern motor leftMotor5;
extern motor rightMotor1;
extern motor rightMotor2;
extern motor rightMotor3;
extern motor rightMotor4;
extern motor rightMotor5;
extern motor bottomArmMotor1;
extern motor bottomArmMotor2;
extern motor topArmMotor1;
extern motor topArmMotor2;

//VEX motor group initializations
extern motor_group leftMotorGroup;
extern motor_group rightMotorGroup;
extern motor_group bottomArmMotorGroup;
extern motor_group topArmMotorGroup;

extern drivetrain myDriveTrain;
void vexcodeInit(void);