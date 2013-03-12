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
	bmdiameter = 1e-6; // 1 um
	bmdiffusion_coefficient = diffusion_coefficient(bmtemperature_K, bmviscosity_eta, bmdiameter);
	bmdiffusion_coefficient = 1e-9;
}

Vector BrownianMotion::get_move()
{	
	static const double SCALER = displacement_factor(bmdimensions, bmdiffusion_coefficient, bmtau);
	return get_move(SCALER);
}

Vector BrownianMotion::get_move(double scale)
{
	static double sqrt_of_3 = sqrt(3);
	scale /= sqrt_of_3;
	double dx = normal(0., 1.*scale);
	double dy = normal(0., 1.*scale);
	double dz = bmdimensions == 2 ? 0.0 : normal(0., 1.*scale);
	return Vector(dx, dy, dz);
}
