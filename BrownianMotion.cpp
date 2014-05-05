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
	scale /= sqrt(3);

	std::random_device rd;
	bmgenerator = std::mt19937(rd());
	bmdistribution = std::normal_distribution<double>(0., 1.*scale);
	bmdistribution_data.reserve(MAX_DIST_ELEMENTS);
	for(int i = 0; i < MAX_DIST_ELEMENTS; ++i)
	{
		bmdistribution_data.push_back(bmdistribution(bmgenerator));
	}

	// when getting close to end of normal distribution for 3 dimensions we must allow
	//  iteratot setup for at least 3 elements, hence MAX_DIST_ELEMENTS-3
	bmuniform = std::uniform_int_distribution<int>(0,MAX_DIST_ELEMENTS-3);

	bmiteration = 0;
}

Vector BrownianMotion::get_move()
{	
	if (bmiteration > MAX_DIST_ELEMENTS-3)
	{
		bmiteration = bmuniform(bmgenerator);
	}

	double dx = bmdistribution_data.at(bmiteration++);
	double dy = bmdistribution_data.at(bmiteration++);
	double dz = bmdimensions == 2 ? 0.0 : bmdistribution_data.at(bmiteration++);

	return Vector(dx, dy, dz);
}
