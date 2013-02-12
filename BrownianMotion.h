#ifndef SIM_BROWNIAN_MOTION_H
#define SIM_BROWNIAN_MOTION_H

#include "Vector.h"

class BrownianMotion {
private:
	int bmdimensions;
public:
	BrownianMotion(int dimensions = 3);
	Vector get_move();
	Vector get_move(double scaler);
};

#endif /* SIM_BROWNIAN_MOTION_H */
