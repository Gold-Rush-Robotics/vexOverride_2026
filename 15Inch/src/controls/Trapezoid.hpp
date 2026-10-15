#pragma once
#include "vex.h"
#include <cmath>

class TrapezoidProfile {
    private:
    bool negative = false;

    double a_max;
    double v_max, v_limit;
    double d_accel, d_cruise, d_total;
    double t_accel, t_cruise, t_total;

    void compute_profile() {

        const double d_tri = (pow(v_limit, 2) / a_max);

        // Account for short distances where Vmax cannot be reached
        if (d_total <= d_tri) {
            v_max = sqrt(a_max * d_total);
        } else {
            v_max = v_limit;
        }

        // Formulas for trapezoidal motion profile
        t_accel = v_max / a_max; 
        d_accel = 0.5 * a_max * pow(t_accel, 2); 
        d_cruise = d_total - (2.0 * d_accel); 
        
        if (d_cruise < 0.0) {
            d_cruise = 0.0;
        }

        t_cruise = d_cruise / v_max; 
        t_total = t_cruise + (2.0 * t_accel); 
    }

    public:
    TrapezoidProfile(double maxAccel, double maxVel, double totalDist)
        : a_max(maxAccel), v_limit(maxVel), d_total(fabs(totalDist)) {

        negative = (totalDist < 0);

        if (d_total == 0.0 || a_max <= 0.0 || v_limit <= 0.0) {
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

        // Piecewise bounds strictly matched to B1-B5
        if (t <= 0.0) { 
            // B1: t == 0
            pos = 0.0;
        } else if (t > 0.0 && t < t_accel) { 
            // B2: (t > 0) && (t < t_accel)
            pos = 0.5 * a_max * pow(t, 2);
        } else if (t >= t_accel && t < (t_cruise + t_accel)) { 
            // B3: (t >= t_accel) && (t < t_cruise + t_accel)
            pos = d_accel + (v_max * (t - t_accel));
        } else if (t >= (t_accel + t_cruise) && t < t_total) { 
            // B4: (t >= t_accel + t_cruise) && (t < t_total)
            pos = d_accel + d_cruise + (v_max * (t - (t_cruise + t_accel))) + (0.5 * (-a_max) * pow(t - (t_cruise + t_accel), 2));
        } else if (t >= t_total) { 
            // B5: (t >= t_total)
            pos = d_total;
        }

        if (negative) {
            pos *= -1.0;
        }

        return pos;
    }
};