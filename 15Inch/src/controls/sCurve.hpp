#include "vex.h"
#include <cmath>

class sCurveProfile {
    private:
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
        calc_jerkPhase();
        calc_accelPhase();
        compute_profile();
    }
};