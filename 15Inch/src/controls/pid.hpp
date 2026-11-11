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
    double start_integral;

    // Bounds integral error range
    double anti_windup;

    // Diminishes big changes like oscillation
    double derivative = 0.0;

    // Stores the output value
    double result = 0.0;

    // Initializing PID constants
    double kP, kI, kD, kT;

    // Clamping the output
    double min_out, max_out;

    // Checks for wrapping
    bool is_angular;

    bool first_update = true;

    static double wrap180(double angle) {
        angle = fmod(angle + 180.0, 360.0);

        if (angle < 0) {angle += 360.0;}

        return angle - 180.0;
    }

    double rawResult() {
        return (kP * error) + (kI * integral) + (kD * derivative);
    }

    public:
    // Class constructor
    PID(double kP, double kI, double kD, double start_integral, double anti_windup, double min_out, double max_out, bool is_angular = true) :
        kP(kP), kI(kI), kD(kD), start_integral(start_integral), anti_windup(anti_windup), min_out(min_out), max_out(max_out), is_angular(is_angular) {}

    // Setter Functions

    // Getter Functions
    double getResult() const { return result; }
    double getError() const { return error; }

    // Derivative related function(s)
    double calcD(double dt) {
        if (first_update) {
            first_update = false;
            return 0.0;
        }

        double change = error - last_error;

        if (is_angular) {change = wrap180(change);}
        
        return change / dt;
    }

    // Integral related function(s)
    double calcI(double dt) {
        bool in_zone = (start_integral <= 0.0) || (fabs(error) < start_integral);
        bool crossed_zero = (error * last_error) < 0.0;

        if (!in_zone || crossed_zero) {
            return 0.0;
        }

        double raw = rawResult();
        bool pushing_into_limit = (raw > max_out && error > 0.0) || (raw < min_out && error < 0.0);

        if (pushing_into_limit) {
            return integral;
        }

        return integral + (error*dt);

    }
    
    double clampI(double integral) {
        if (anti_windup <= 0.0) {return integral;}

        return fmax(-anti_windup, fmin(integral, anti_windup));
    }

    // Output related function(s)
    double clampResult(double result) {
        return fmax(min_out, fmin(result, max_out));
    }

    // Updating PID and tracking errors
    double update(double current_error, double dt) {
        if (dt <= 0.0) {return result;}

        if (is_angular) {
            error = wrap180(current_error);
        } else {
            error = current_error;
        }

        derivative = calcD(dt);
        integral = clampI(calcI(dt));
        last_error = error;

        result = clampResult(rawResult);

        return result;
    }

    void reset() {
        integral = 0.0;
        derivative = 0.0;
        last_error = 0.0;
        error = 0.0;
        result = 0.0;
        first_update = true;
    }

    // Retune without recreating object
    void setGains(double p, double i, double d) {
        kP = p;
        kI = i;
        kD = d; 
    }
};