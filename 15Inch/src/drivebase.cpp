#include "vex.h"
#include "vex_imu.h"
#include "controls/drivebase.hpp"
#include "controls/pid.hpp"
#include <cmath>

using namespace vex;

void turnAuto(PID& turnPID);

int main() {
  // Initialize the VEX system and robot config before the control loop starts.
  vexcodeInit();

  // Create the robot drivetrain controller and a PID loop for autonomous turns.
  DriveBase driveBase;
  PID drivePID(0.32, 0.0, 0.046, 5.0, 50.0, -100.0, 100.0, true);
  bool lastButtonAState = false;

  /*
  ===============================================
                  Tests
  (kP,  kI,  kD)  - Reason
  (0.3 ,0.0 ,0.0) - Overshoots by 1-5 degrees, oscillates for 1-2 times and settles
  
  =============================================== 
  
  */

  // Main robot loop: keep the manual drive system alive and check for auto-turn trigger.
  while (true) {
    // Toggle between tank and arcade drive based on controller input.
    driveBase.updateDriveMode();
    // Apply whichever drive mode is currently selected to the motors.
    driveBase.driveControl();

    // Detect a single button press on A to start the automated turn routine once.
    bool buttonAState = Controller1.ButtonA.pressing();
    if (buttonAState && !lastButtonAState) {
      turnAuto(drivePID);
    }
    lastButtonAState = buttonAState;

    // Small delay keeps the loop responsive without starving other robot tasks.
    wait(20, msec);
  }
}

// Autonomously rotates the robot to a target heading using a PID loop and the inertial sensor.
void turnAuto(PID& drivePID) {
  // Tell the driver that the robot is calibrating the inertial sensor before turning.
  Brain.Screen.print("Calibrating inertial sensor");
  Inertial.calibrate();
  Inertial.resetRotation();
  Inertial.resetHeading();
  waitUntil(!Inertial.isCalibrating());
  Brain.Screen.clearScreen();
  Brain.Screen.setCursor(0,0);

  // These constants define the turn behavior: 180-degree target, tight tolerance, timeout, and
  // how many consecutive stable cycles are required before the turn is considered finished.
  const double targetDegrees = 180.0;
  const double toleranceDegrees = 1.5;
  const double timeoutSeconds = 5.0;
  const int requiredSettledCycles = 5;
  int settledCycles = 0;
  timer turnTimer;
  timer loopTimer;

  // Stop both sides before resetting the sensor and controller state.
  leftMotorGroup.stop();
  rightMotorGroup.stop();
  Inertial.resetRotation();
  drivePID.reset();
  turnTimer.reset();
  loopTimer.reset();

  // Keep turning until either the timeout expires or the robot has stayed near the target heading.
  while (turnTimer.time(timeUnits::sec) < timeoutSeconds) {
    // Error is how far off from the desired heading the robot currently is.
    double error = targetDegrees - Inertial.rotation(rotationUnits::deg);
    double dt = loopTimer.time(timeUnits::sec);
    loopTimer.reset();
    if (dt < 0.001) {
      dt = 0.02;
    }

    // PID calculates a motor command based on the heading error and time interval.
    double output = drivePID.update(error, dt);

    // When the robot is close enough to the target, stop and count a "settled" cycle.
    if (fabs(error) <= toleranceDegrees) {
      leftMotorGroup.stop(hold);
      rightMotorGroup.stop(hold);
      settledCycles++;
      if (settledCycles >= requiredSettledCycles) {
        break;
      }
    } else {
      // A turn that is not stable resets the settled counter and keeps driving toward the target.
      settledCycles = 0;

      // Use full speed at the start of the turn to get moving quickly, but back off aggressively as
      // the robot nears the target to reduce overshoot and oscillation near the setpoint.
      const double maxTurnSpeed = 100.0;
      const double minTurnSpeed = 15.0;
      double speed = maxTurnSpeed;

      if (fabs(error) < 45.0) {
        speed = fabs(output);
        if (speed < minTurnSpeed) {
          speed = minTurnSpeed;
        }
        if (speed > maxTurnSpeed) {
          speed = maxTurnSpeed;
        }
      } else if (fabs(error) < 90.0) {
        speed = 60.0;
      }

      // Keep the robot from dithering right at the heading target by forcing a tiny output to zero.
      if (fabs(output) < 2.0) {
        speed = 0.0;
      }

      // Drive command is proportional to the PID output; the sign decides which direction to spin.
      leftMotorGroup.setVelocity(speed, percentUnits::pct);
      rightMotorGroup.setVelocity(speed, percentUnits::pct);

      if (output > 0.0) {
        // Positive output means rotate rightward; left side reverses while right side spins forward.
        leftMotorGroup.spin(reverse);
        rightMotorGroup.spin(forward);
      } else {
        // Negative output means rotate leftward; opposite motor directions are used.
        leftMotorGroup.spin(forward);
        rightMotorGroup.spin(reverse);
      }
    }

    wait(20, msec);
  }

  // Stop the drivetrain and report whether the turn succeeded or timed out.
  leftMotorGroup.stop(hold);
  rightMotorGroup.stop(hold);
  Controller1.Screen.clearLine();
  Controller1.Screen.setCursor(1, 1);
  if (settledCycles >= requiredSettledCycles) {
    Controller1.Screen.print("Turn: ");
  } else {
    Controller1.Screen.print("Turn timeout: ");
  }
  Controller1.Screen.print(Inertial.rotation(rotationUnits::deg));
}
