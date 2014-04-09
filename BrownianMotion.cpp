#include <cstdlib>
#include <cmath>

#include "BrownianMotion.h"
#include "Vector.h"
#include "Math.h"
#include "tri_logger/tri_logger.hpp"

BrownianMotion::BrownianMotion(int dimensions, double tau)
{
	bmdimensions = dimensions;
	bmtau = tau;

	bmtemperature_K = 293.; // 20 deg Celsius
	bmviscosity_eta = 1e-3; // water
	//bmdiameter = 1e-6; // 1 um
	//bmdiffusion_coefficient = diffusion_coefficient(bmtemperature_K, bmviscosity_eta, bmdiameter);
	bmdiffusion_coefficient = 1e-9;

	scale = displacement_factor(bmdimensions, bmdiffusion_coefficient, bmtau);
	scale /= sqrt(3);;

	std::random_device rd;
	generator = std::mt19937(rd());
	distribution = std::normal_distribution<double>(0., 1.*scale);
}

Vector BrownianMotion::get_move()
{	
	double dx = distribution(generator);
	double dy = distribution(generator);
	double dz = bmdimensions == 2 ? 0.0 : distribution(generator);
	return Vector(dx, dy, dz);
}
