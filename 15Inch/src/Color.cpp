#include "vex.h"

using namespace vex;

void runBlueSurfaceRoutine() {
	Optical.setLightPower(100, percent);

	constexpr double wheelDiameterMm = 101.6; // 4-inch wheel
	constexpr double travelMm = 1500.0;
	constexpr double blueHueMin = 180.0;
	constexpr double blueHueMax = 240.0;
	const double motorDegrees = travelMm / (wheelDiameterMm * 3.141592653589793) * 360.0;

	const double hue = Optical.hue();
	if (hue >= blueHueMin && hue <= blueHueMax) {
		leftMotorGroup.spinFor(fwd, motorDegrees, degrees, false);
		rightMotorGroup.spinFor(fwd, motorDegrees, degrees, true);
		leftMotorGroup.stop(brake);
		rightMotorGroup.stop(brake);

		// Wait for the blue surface to pass before triggering again.
		double detectedHue = Optical.hue();
		while (detectedHue >= blueHueMin && detectedHue <= blueHueMax) {
			wait(20, msec);
			detectedHue = Optical.hue();
		}
	}
}
// tried to download but keeps running sai's code for some reason with the braibn displaying gps.