#include <cmath>
#include <cassert>

class PID{
    private:
    // Stores error from previous iteration
    double last_error = 0.0;
    
    // Stores current error
    double error = 0.0;

    // Correct deviations or prevents thems
    double integral = 0.0;

    // Accumulating integral when error passes a certain value
    double start_integral = 0.0;

    // Bounds integral error range
    double anti_windup;

    // Diminishes big changes like oscillation
    double derivative = 0.0;

    // Stores the output value
    double result;

    // Initializing PID constants
    double kP, kI, kD, kT;

    // Clamping the output
    double min_out, max_out;

    // Checks for wrapping
    bool is_angular;

    static double wrap180(double angle) {
        angle = std::fmod(angle + 180.0, 360.0);

        if (angle < 0) {angle += 360.0;}

        return angle - 180.0;
    }

    public:
    // Class constructor
    PID(double kP, double kI, double kD, double start_integral, double anti_windup, double min_out, double max_out, bool is_angular = true) :
        kP(kP), kI(kI), kD(kD), start_integral(start_integral), anti_windup(anti_windup), min_out(min_out), max_out(max_out), is_angular(is_angular) {}

    // Setter Functions

    // Getter Functions
    double getResult() const { return result; }
    double getError() const { return error; }

    // Derivative related functions
    double calcD(double dt) {
        return ((error - last_error) / dt);
    }

    // Integral related function
    double calcI(double dt) {
        if (start_integral > 0.0 && std::fabs(error) < start_integral) {
            integral += error * dt;
        }

        return integral;
    }
    
    double clampI(double integral) {
        return fmax(-anti_windup, fmin(integral, anti_windup));
    }

    // Output related function
    double clampResult() {
        return fmax(min_out, fmin(result, max_out));
    }

    // Updating PID and tracking errors
    double update(double current_error, double dt) {
        assert(dt > 0.0 && "PID: dt must be greater than 0");

        if (is_angular) {
            error = wrap180(current_error);
        } else {
            error = current_error;
        }

        integral = calcI(dt);

        if (anti_windup > 0.0) {
            integral = clampI(integral);
        }

        derivative = calcD(dt);
        last_error = error;

        result = (kP * error) + (kI * integral) + (kD * derivative);
        
        return clampResult();
    }

    void reset() {
        integral = 0.0;
        last_error = 0.0;
        result = 0.0;
    }

    // Retune without recreating object
    void setGains(double p, double i, double d) {
        kP = p;
        kI = i;
        kD = d; 
    }
};