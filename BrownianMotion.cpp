#include <ctime>
#include <cstdlib>

#include "BrownianMotion.h"
#include "Translation.h"

BrownianMotion::BrownianMotion()
{
	srand ( time(NULL) );
}

Translation BrownianMotion::get_move()
{
	static const float SCALER = 130;
	int dx = ((float)rand()/RAND_MAX - 0.5) * SCALER;
	int dy = ((float)rand()/RAND_MAX - 0.5) * SCALER;
	int dz = ((float)rand()/RAND_MAX - 0.5) * SCALER;
	return Translation(dx, dy, dz);
}
