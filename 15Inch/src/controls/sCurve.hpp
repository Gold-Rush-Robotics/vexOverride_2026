#pragma once
#include "vex.h"
#include <cmath>

class sCurveProfile {
    private:
    bool negative = false;

    double j_const, a_max;
    double v_p1, v_max, v_limit;   // v_limit: caller's cap, never modified
    double d_jerkUp, d_jerkDown, d_accel, d_cruise, d_total;
    double t_jerk, t_accel, t_cruise, t_total;

    void calc_jerkPhase() {
        t_jerk = a_max / j_const;

        v_p1 = 0.5 * j_const * pow(t_jerk, 2);

        d_jerkUp = j_const * pow(t_jerk, 3) / 6;
    }

    void calc_accelPhase() {
        t_accel = (v_max - 2*v_p1) / a_max;
        if (t_accel < 0.0) {t_accel = 0.0;}   // float noise in the jerk-only case

        d_accel = (v_p1 * t_accel) + (0.5 * a_max * pow(t_accel, 2));

        d_jerkDown = ((v_max - v_p1) * t_jerk) + (2 * d_jerkUp);
    }

    void calc_cruisePhase() {
        d_cruise = d_total - 2*(d_jerkUp + d_accel + d_jerkDown);
        if (d_cruise < 0.0) {d_cruise = 0.0;}   // float noise when there is no cruise

        t_cruise = d_cruise / v_max;
    }

    void calc_totalTime() {
        t_total = t_cruise + (2*t_accel) + (4*t_jerk);
    }

    void compute_profile() {
        calc_jerkPhase();

        // Can't finish the jerk ramps at a_max
        const double d_jerkOnly = 2 * pow(a_max, 3) / pow(j_const, 2);
        // Ramps finish, but the move ends before reaching v_limit
        const double d_noCruise = v_limit * (a_max/j_const + v_limit/a_max);

        if (d_total < d_jerkOnly) {
            // Shrink a_max until the move is exactly two jerk-only sides
            a_max = cbrt(0.5 * d_total * pow(j_const, 2));
            calc_jerkPhase();

            v_max = 2 * v_p1;

        } else if (d_total < d_noCruise) {
            // Solve v² + a·T·v − a·d = 0 for the peak velocity
            v_max = 0.5 * a_max * (-t_jerk + sqrt(pow(t_jerk, 2) + 4*d_total/a_max));

        } else {
            v_max = v_limit;
        }

        calc_accelPhase();
        calc_cruisePhase();
        calc_totalTime();
    }

    public:
    sCurveProfile(double const_jerk, double maxAccel, double maxVel, double totalDist)
        : j_const(const_jerk), a_max(maxAccel), v_limit(maxVel), d_total(fabs(totalDist)) {

        negative = (totalDist < 0);

        if (d_total == 0.0) return;   // nothing to plan; everything stays 0

        // Cap below a_max²/j: the cap is hit before a_max is ever reached
        if (v_limit < pow(a_max, 2) / j_const) {
            a_max = sqrt(j_const * v_limit);
        }

        compute_profile();
    }


    double getPosition(double t) {
        double pos = 0.0;
        double tau;

        if ( t==0 ) {

            pos = 0.0;

        } else if ( (t > 0) && (t < t_jerk) ) {
            // Jerk-Up Phase

            pos = j_const*pow(t, 3)/6;
        
        } else if ( (t >= t_jerk) && (t < (t_jerk+t_accel))) {
            // Const Accel Phase

            tau = t - t_jerk;
            pos = d_jerkUp + (v_p1*tau) + (0.5 * a_max * pow(tau, 2));

        } else if ( (t >= (t_jerk+t_accel)) && (t < (2*t_jerk + t_accel)) ) {
            // Jerk-Down Phase

            tau = t - t_jerk - t_accel;
            pos = d_jerkUp + d_accel + ((v_max - v_p1)*tau) + (0.5 * a_max * pow(tau, 2)) - (j_const*pow(tau, 3)/6);

        } else if ( (t >= (2*t_jerk + t_accel)) && (t < (2*t_jerk + t_accel + t_cruise)) ) {
            // Cruise Phase
            
            tau = t - 2*t_jerk - t_accel;
            pos = d_jerkUp + d_accel + d_jerkDown + (v_max*tau);

        } else if ( (t >= (2*t_jerk + t_accel + t_cruise)) && (t < (t_total - t_jerk - t_accel)) ) {
            // Jerk-Down Phase 2

            tau = t - 2*t_jerk - t_accel - t_cruise;
            pos = d_jerkUp + d_accel + d_jerkDown + d_cruise + (v_max*tau) -  (j_const*pow(tau, 3)/6);

        } else if ( (t >= (t_total - t_jerk - t_accel)) && (t < (t_total - t_jerk)) ) {
            // Const Accel Phase
            
            tau = t + t_jerk + t_accel - t_total;
            pos = d_jerkUp + d_accel + 2*d_jerkDown + d_cruise + ((v_max-v_p1)*tau) - (0.5 * a_max * pow(tau, 2));

        } else if ( (t >= (t_total - t_jerk)) && (t < t_total)) {
            // Jerk-Down Phase 2

            tau = t + t_jerk - t_total;
            pos = 2*(d_jerkDown + d_accel) + d_jerkUp + d_cruise + (v_p1*tau) - (0.5 * a_max * pow(tau, 2)) + (j_const*pow(tau, 3)/6);

         } else if ( t >= t_total ) {

            pos = d_total;

        }

        pos = negative ? (-1*pos) : (1*pos);

        return pos;
    }

};