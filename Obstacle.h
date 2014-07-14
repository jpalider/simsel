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
	Obstacle(Id identifier, Vector position, Coordinate size_x, Coordinate size_y, Coordinate size_z);
	bool collide(Molecule *m);
	void handle_collision(Molecule *m);

	friend std::ostream & operator<<(std::ostream &os, const Obstacle& c);

};
#endif /* SIM_OBSTACLE_H */
