using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor leftMotor1;
extern motor leftMotor2;
extern motor leftMotor3;
extern motor rightMotor1;
extern motor rightMotor2;
extern motor rightMotor3;

//VEX motor group initializations
extern motor_group leftMotorGroup;
extern motor_group rightMotorGroup;
extern drivetrain DriveTrain;

//VEX sensors
extern inertial Inertial;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);