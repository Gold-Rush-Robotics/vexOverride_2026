#include "vex.h"

using namespace vex;

// Creating a global competition instance
competition Competition;

// Define: global instances of motors here

// Pre-Auton functions are to be performed before the competition starts.
void pre_auton(void) {
    // Initializing the robot config.
    vexcodeInit();

    // Insert and define activities that should occur before comp
}

// Auton functions to control robot during the autonomous phase
void auton(void) {

}

// Driver control function for the drive control phase
void usercontrol(void) {
    // Main execution loops for this function
    while(1) {
        
        wait(20, msec);
    }
}

// Main function will set up the competition functions and callbacks
int main() {
    Competition.autonomous(auton);
    Competition.drivercontrol(usercontrol);

    // Run the pre-auton function
    pre_auton();

    // Prevents main from exiting with an infinite loop
    while (true) {
        wait(100, msec);
    }
}