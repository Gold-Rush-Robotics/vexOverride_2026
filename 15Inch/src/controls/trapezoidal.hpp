#pragma once
#include <cmath>
#include <algorithm>

class trapezoidalProfile {
    private:
    double aMax;
    double vMax;
    double tAccel, tCruise, tTotal;
    double dAccel, dCruise, dTotal;
    bool negative = false;

    void calc_accelPhase() { 
        tAccel = vMax/aMax;

        dAccel =  vMax*tAccel/2; 
    }

    void calc_cruisePhase() { 
        dCruise = dTotal-(2*dAccel);

        tCruise = dCruise/vMax;
    }

    void calc_tTotal() {
        tTotal = tCruise + (2*tAccel);
    }

    void computeProfile() {

        if ((2*dAccel) > dTotal) {
            vMax = sqrt(2*aMax*dTotal);

            calc_accelPhase();
        }

        calc_cruisePhase();
        calc_tTotal();
    }

    public:
    trapezoidalProfile(double maxAccel, double distTotal, double maxVel): aMax(maxAccel), vMax(maxVel) {
        this->dTotal = abs(distTotal);
        negative = (distTotal < 0) ? true : false;

        calc_accelPhase();
        computeProfile();
    };

    double get_tTotal() { return tTotal; }
    double get_vMax() { return vMax; }

    double getPos(double t) {
        double pos = 0.0;

        if (t == 0) {
            pos = 0.0;
        } else if ((t > 0) && (t < tAccel)) {

            pos = 0.5 * aMax * pow(t, 2);

        } else if ((t >= tAccel) && (t < (tAccel+tCruise))) {
            
            pos = dAccel + (vMax * (t - tAccel));

        } else if ((t >= (tAccel+tCruise)) && (t < tTotal)) {
            
            double tDecel =  tAccel + tCruise;

            pos = dAccel + dCruise + (vMax*(t-tDecel)) - (0.5*aMax*pow((t-tDecel), 2));
        
        } else if (t >= tTotal) {
            
            pos = dTotal;

        }

        pos = negative ? (-1*pos) : pos;

        return pos;
    }
};