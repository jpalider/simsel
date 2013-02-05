#include <cstdlib>

#include "BrownianMotion.h"
#include "Vector.h"
#include "Math.h"

BrownianMotion::BrownianMotion()
{
}

Vector BrownianMotion::get_move()
{	
	static const float SCALER = 1.0f;
	return get_move(SCALER);
}

Vector BrownianMotion::get_move(float scale)
{
	// float dx = ((float)rand()/RAND_MAX - 0.5) * scale;
	// float dy = ((float)rand()/RAND_MAX - 0.5) * scale;
	// float dz = ((float)rand()/RAND_MAX - 0.5) * scale;
	float dx = normal(0, 1);
	float dy = normal(0, 1);
	float dz = 0f;
	return Vector(dx*scale, dy*scale, dz*scale);
}
