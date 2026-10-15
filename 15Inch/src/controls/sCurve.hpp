#pragma once
#include "vex.h"
#include <cmath>

class sCurveProfile {
    private:
    bool negative = false;

    double a_max;
    double v_p1, v_max, v_limit;   // v_limit: caller's cap, never modified
    double d_jerkUp, d_jerkDown, d_accel, d_cruise, d_total;
    double t_jerk, t_accel, t_cruise, t_total;

    void compute_profile() {
        // Keep the original variable names, but compute a true trapezoid profile:
        // accelerate at a_max, hold v_max, then decelerate back to zero.
        v_p1 = 0.0;
        d_jerkUp = 0.0;
        d_jerkDown = 0.0;
        t_jerk = 0.0;

        const double d_tri = (pow(v_limit, 2) / a_max);

        if (d_total <= d_tri) {
            v_max = sqrt(a_max * d_total);
            d_accel = 0.5 * (pow(v_max, 2) / a_max);
            d_cruise = 0.0;
            t_accel = v_max / a_max;
            t_cruise = 0.0;
            t_total = 2.0 * t_accel;
        } else {
            v_max = v_limit;
            t_accel = v_max / a_max;
            d_accel = 0.5 * (pow(v_max, 2) / a_max);
            d_cruise = d_total - (2.0 * d_accel);
            if (d_cruise < 0.0) {
                d_cruise = 0.0;
            }
            t_cruise = d_cruise / v_max;
            t_total = t_accel + t_cruise + t_accel;
        }
    }

    public:
    sCurveProfile(double maxAccel, double maxVel, double totalDist)
        : a_max(maxAccel), v_limit(maxVel), d_total(fabs(totalDist)) {

        negative = (totalDist < 0);

        if (d_total == 0.0) {
            v_max = 0.0;
            d_accel = 0.0;
            d_cruise = 0.0;
            t_accel = 0.0;
            t_cruise = 0.0;
            t_total = 0.0;
            return;
        }

        if (a_max <= 0.0 || v_limit <= 0.0) {
            v_max = 0.0;
            d_accel = 0.0;
            d_cruise = 0.0;
            t_accel = 0.0;
            t_cruise = 0.0;
            t_total = 0.0;
            return;
        }

        compute_profile();
    }

    double getPosition(double t) {
        double pos = 0.0;
        double tau;

        if (t <= 0.0) {
            pos = 0.0;
        } else if (t < t_accel) {
            // Acceleration phase
            pos = 0.5 * a_max * pow(t, 2);
        } else if (t < (t_accel + t_cruise)) {
            // Cruise phase
            tau = t - t_accel;
            pos = d_accel + (v_max * tau);
        } else if (t < t_total) {
            // Deceleration phase
            tau = t - (t_accel + t_cruise);
            pos = d_accel + d_cruise + (v_max * tau) - (0.5 * a_max * pow(tau, 2));
        } else {
            pos = d_total;
        }

        if (negative) {
            pos *= -1.0;
        }

        return pos;
    }
};