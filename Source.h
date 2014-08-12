#ifndef SIM_SOURCE_H
#define SIM_SOURCE_H

#include "Boundary.h"
#include "Obstacle.h"
#include "Types.h"
#include "Vector.h"

class Molecule;

class Source : public Boundary
{
public:
	Source(Id identifier, Vector position, Coordinate radius);
	Source(Id identifier, Vector position, Coordinate size_x, Coordinate size_y, Coordinate size_z);
	bool collide(Molecule *m);
	void handle_collision(Molecule *m);
	bool release(long time, MStore *molecules, Obstacle *space = nullptr);
};

#endif /* SIM_SOURCE_H */
