#ifndef SIM_BROWNIAN_MOTION_H
#define SIM_BROWNIAN_MOTION_H

#include <random>
#include "Vector.h"

class BrownianMotion {
private:
	// Environment parameters
	int    bmdimensions;
	double bmdiffusion_coefficient;
	double bmtemperature_K;
	double bmviscosity_eta;
	//double bmdiameter;
	double bmtau;

	double scale;

	// randomness
	std::mt19937 generator;
	std::normal_distribution<double> distribution;

public:
	BrownianMotion(int dimensions = 3, double tau = 1);
	Vector get_move();
};

#endif /* SIM_BROWNIAN_MOTION_H */
