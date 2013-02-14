#ifndef SIM_BROWNIAN_MOTION_H
#define SIM_BROWNIAN_MOTION_H

#include "Vector.h"

class BrownianMotion {
private:
	int bmdimensions;
	double bdiffusion_coefficient;
	double temperature_K;
	double viscosity_eta;
	double diameter;

public:
	BrownianMotion(int dimensions = 3);
	Vector get_move();
	Vector get_move(double scaler);
};

#endif /* SIM_BROWNIAN_MOTION_H */
