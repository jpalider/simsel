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
	static const float SCALER = 1.0f;
	return get_move(SCALER);
}

Vector BrownianMotion::get_move(float scale)
{
	float dx = normal(0.f, 1.f);
	float dy = normal(0.f, 1.f);
	float dz = bmdimensions == 2 ? 0.f : normal(0.f, 1.f);
	return Vector(dx*scale, dy*scale, dz*scale);
}
