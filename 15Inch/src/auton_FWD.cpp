#include "vex.h"
#include "robot-config.h"
#include "controls/pid.hpp"

using namespace vex;

// Absolute targets, carried across moves so leftover error from one move
// is corrected by the next instead of being thrown away
static double targetDistanceMm = 0.0;
static double targetHeadingDeg = 0.0;

static const double wheelTravelMm = 320.0;

static double currentDistanceMm() {
  double leftDistance = leftMotorGroup.position(degrees) / 360.0 * wheelTravelMm;
  double rightDistance = rightMotorGroup.position(degrees) / 360.0 * wheelTravelMm;
  return (leftDistance + rightDistance) / 2.0;
}

// Call once from main(), after vexcodeInit(). Robot must be still.
void initAuton() {
  Inertial.calibrate();
  while (Inertial.isCalibrating()) {
    wait(20, msec);
  }
  leftMotorGroup.setPosition(0, degrees);
  rightMotorGroup.setPosition(0, degrees);
}

// Make "where we are now" the reference. Call at the start of autonomous,
// and before a one-off test move after the robot has been driven by hand.
void syncTargetsToCurrent() {
  targetDistanceMm = currentDistanceMm();
  targetHeadingDeg = Inertial.rotation(degrees);
}

void moveDistancePID(double deltaMm) {
  const double toleranceMm = 8.0;
  const double timeoutSec = 10.0;
  const int requiredSettledCycles = 5;

  targetDistanceMm += deltaMm;   // heading target is left alone: drive straight

  // Starting values, re-tune on the robot.
  // I-zone 100 mm, integral capped at 5000 (kI * 5000 = 10% output)
  PID distancePID(0.125, 0.002, 0.00225, 100.0, 5000.0, -60.0, 60.0, false);
  // rotation() is continuous, so no wrapping needed
  PID headingPID(1.0, 0.0, 0.0, 0.0, 0.0, -20.0, 20.0, false);

  timer moveTimer;
  timer loopTimer;
  int settledCycles = 0;
  int cycles = 0;

  while (settledCycles < requiredSettledCycles && moveTimer.time(sec) < timeoutSec) {
    // Measured loop time, with a fallback for the first pass
    double dt = loopTimer.time(sec);
    loopTimer.reset();
    if (dt < 0.005) { dt = 0.02; }

    double currentDistance = currentDistanceMm();
    double distanceError = targetDistanceMm - currentDistance;
    double headingError = targetHeadingDeg - Inertial.rotation(degrees);

    // Update both PIDs every cycle so their state never goes stale
    double drive = distancePID.update(distanceError, dt);
    double turn = headingPID.update(headingError, dt);

    if (fabs(distanceError) <= toleranceMm) {
      leftMotorGroup.stop(hold);
      rightMotorGroup.stop(hold);
      settledCycles++;
    } else {
      settledCycles = 0;
      // Positive heading error = need to turn clockwise = left side faster.
      // If the robot veers further off instead of correcting, swap the signs.
      leftMotorGroup.spin(forward, drive + turn, velocityUnits::pct);
      rightMotorGroup.spin(forward, drive - turn, velocityUnits::pct);
    }

    // Print every 10th cycle only; screen writes slow the loop down
    if (cycles % 10 == 0) {
      Brain.Screen.clearScreen();
      Brain.Screen.setCursor(1, 1);
      Brain.Screen.print("Dist err: %.1f mm  Head err: %.1f deg", distanceError, headingError);
    }

    cycles++;
    wait(20, msec);
  }

  leftMotorGroup.stop(hold);
  rightMotorGroup.stop(hold);
}

// Test moves for the controller buttons
void moveForward1000mm() {
  syncTargetsToCurrent();
  moveDistancePID(2000.0);
}

void moveBackward1000mm() {
  syncTargetsToCurrent();
  moveDistancePID(-2000.0);
}

void autonomous(void) {
  syncTargetsToCurrent();
  moveDistancePID(2000.0);
  // further moves here keep the same reference, so error does not stack up
}