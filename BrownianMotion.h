#ifndef SIM_BROWNIAN_MOTION_H
#define SIM_BROWNIAN_MOTION_H

#include "Vector.h"

class BrownianMotion {
private:

public:
	BrownianMotion();
	Vector get_move();
	Vector get_move(float scaler);
};

#endif /* SIM_BROWNIAN_MOTION_H */
