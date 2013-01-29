#include <ctime>
#include <cstdlib>

#include "BrownianMotion.h"
#include "Vector.h"

BrownianMotion::BrownianMotion()
{
	srand ( time(NULL) );
}

Vector BrownianMotion::get_move()
{	
	static const float SCALER = 130;
	return get_move(SCALER);
}

Vector BrownianMotion::get_move(float scale)
{
	float dx = ((float)rand()/RAND_MAX - 0.5) * scale;
	float dy = ((float)rand()/RAND_MAX - 0.5) * scale;
	float dz = ((float)rand()/RAND_MAX - 0.5) * scale;
	dz = 0;
	return Vector(dx, dy, dz);
}
