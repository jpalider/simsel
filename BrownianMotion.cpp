#include <cstdlib>

#include "BrownianMotion.h"
#include "Vector.h"
#include "Math.h"
#include "tri_logger/tri_logger.hpp"

BrownianMotion::BrownianMotion(int dimensions)
{
	bmdimensions = dimensions;
}

Vector BrownianMotion::get_move()
{	
	static const double SCALER = 1.0;
	return get_move(SCALER);
}

Vector BrownianMotion::get_move(double scale)
{
	double dx = normal(0., 1.);
	double dy = normal(0., 1.);
	double dz = bmdimensions == 2 ? 0.0 : normal(0., 1.);
	return Vector(dx*scale, dy*scale, dz*scale);
}
