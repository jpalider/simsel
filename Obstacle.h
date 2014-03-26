#ifndef SIM_OBSTACLE_H
#define SIM_OBSTACLE_H

#include "Types.h"
#include "Boundary.h"
#include "tri_logger/tri_logger.hpp"

class Obstacle : public Boundary
{
private:

public:
	Obstacle(Id identifier, Vector position, Coordinate radius);
	bool collide(Molecule *m);

};
#endif /* SIM_OBSTACLE_H */
