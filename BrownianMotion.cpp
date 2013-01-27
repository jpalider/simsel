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
	int dx = ((float)rand()/RAND_MAX - 0.5) * SCALER;
	int dy = ((float)rand()/RAND_MAX - 0.5) * SCALER;
	int dz = ((float)rand()/RAND_MAX - 0.5) * SCALER;
	return Vector(dx, dy, dz);
}
