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
	static const int MAX_DIST_ELEMENTS = 1*1000*1000;

	std::mt19937 bmgenerator;
	std::normal_distribution<double> bmdistribution;
	std::vector<double> bmdistribution_data;

	std::uniform_int_distribution<int> bmuniform;

	int bmiteration;
public:
	BrownianMotion(int dimensions = 3, double tau = 1);
	Vector get_move();
};

#endif /* SIM_BROWNIAN_MOTION_H */
