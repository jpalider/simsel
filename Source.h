#ifndef SIM_SOURCE_H
#define SIM_SOURCE_H

#include "Boundary.h"
#include "Types.h"
#include "Vector.h"

class Molecule;

class Source : public Boundary
{
public:
	Source(Id identifier, Vector position, Coordinate radius);
	bool collide(Molecule *m);
	void handle_collision(Molecule *m);
	void release(Molecule *m);
};

#endif /* SIM_SOURCE_H */
