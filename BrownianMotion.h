#ifndef SIM_BROWNIAN_MOTION_H
#define SIM_BROWNIAN_MOTION_H

#include "Vector.h"

class BrownianMotion {
private:
	int bmdimensions;
	double bmdiffusion_coefficient;
	double bmtemperature_K;
	double bmviscosity_eta;
	double bmdiameter;
	double bmtau;

public:
	BrownianMotion(int dimensions = 3, double tau = 1);
	Vector get_move();
	Vector get_move(double scaler);
};

#endif /* SIM_BROWNIAN_MOTION_H */
