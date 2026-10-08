#include "vex.h"
#include <cmath>

class sCurveProfile {
    private:
    bool negative;

    double jConst;
    double aMax;
    double vP1, vMax;
    double dJerk, dAccel, dCruise, dTotal;
    double tJerk, tAccel, tCruise, tTotal;

    void calc_jerkPhase() {
        tJerk = aMax / jConst;

        vP1 = 0.5 * jConst * pow(tJerk, 2);

        dJerk = jConst*pow(tJerk, 3) / 6;
    }
    
    void calc_accelPhase() {
        tAccel = (vMax - 2*vP1) / aMax;

        dAccel = (vP1*tAccel) + (0.5 * aMax * pow(tAccel, 2));
    }

    void calc_cruisePhase() {
        dCruise = dTotal - ((2*dAccel) + (4*dJerk));

        tCruise = dCruise / vMax;
    }

    void calc_totalTime() {
        tTotal = tCruise + (2*tAccel) + (4*tJerk);
    }

    void compute_profile() {
        if (dTotal < 4*dJerk) {
            
            aMax = cbrt(1.5 * dTotal * pow(jConst, 2));

            calc_jerkPhase();

            vMax = 2*vP1;
            tAccel = 0.0;
            dAccel = 0.0;

        } else if (dTotal < (4*dJerk + 2*dAccel)) {

            tAccel = (-vP1 + sqrt(pow(vP1, 2) + aMax*(dTotal - 4*dJerk)))/aMax;
            vMax = 2*vP1 + aMax*tAccel;
            dAccel = (vP1*tAccel) + (0.5 * aMax * pow(tAccel, 2));

        }

        calc_cruisePhase();
        calc_totalTime();
    }

    public:

    sCurveProfile(double constJerk, double maxAccel, double maxVel, double totalDist): jConst(constJerk), aMax(maxAccel), vMax(maxVel), dTotal(totalDist) {
        this->dTotal = totalDist;
        negative = (totalDist < 0) ? true:false;
        
        calc_jerkPhase();
        calc_accelPhase();
        compute_profile();
    }

    double getPosition(double t) {
        double pos;
        double tau;

        if ( t==0 ) {

            pos = 0.0;

        } else if ( (t > 0) && (t < tJerk) ) {
            // Jerk-Up Phase

            pos = jConst*pow(t, 3)/6;
        
        } else if ( (t >= tJerk) && (t < (tJerk+tAccel))) {
            // Const Accel Phase

            tau = t - tJerk;
            pos = dJerk + (0.5 * aMax * pow(tau, 2));

        } else if ( (t >= (tJerk+tAccel)) && (t < (2*tJerk + tAccel)) ) {
            // Jerk-Down Phase

            tau = t - tJerk - tAccel;
            pos = dJerk + dAccel - (jConst*pow(tau, 3)/6);

        } else if ( (t >= (2*tJerk + tAccel)) && (t < (2*tJerk + tAccel + tCruise)) ) {
            // Cruise Phase
            
            tau = t - 2*tJerk - tAccel;
            pos = 2*dJerk + dAccel + (vMax*tau);

        } else if ( (t >= (2*tJerk + tAccel + tCruise)) && (t < (tTotal - tJerk - tAccel)) ) {
            // Jerk-Up Phase 2

            tau = t - 2*tJerk - tAccel - tCruise;
            pos = 2*dJerk + dAccel + dCruise +  (jConst*pow(tau, 3)/6);

        } else if ( (t >= (tTotal - tJerk - tAccel)) && (t < (tTotal - tJerk)) ) {
            // Const Accel Phase
            
            tau = t + tJerk + tAccel - tTotal;
            pos = 3*dJerk + dAccel + dCruise + (0.5 * aMax + pow(tau, 2));

        } else if ( (t >= (tTotal - tJerk)) && (t < tTotal)) {
            // Jerk-Down Phase 2

            tau = t + tJerk - tTotal;
            pos = 3*dJerk + 2*dAccel + dCruise - (jConst*pow(tau, 3)/6);

         } else if ( t >= tTotal ) {

            pos = dTotal;

        }

        pos = negative ? (-1*pos) : (1*pos);

        return pos;
    }

};