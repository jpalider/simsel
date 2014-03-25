#ifndef SIM_SOURCE_H
#define SIM_SOURCE_H

#include "Types.h"
#include "Vector.h"

class Molecule;

class Source
{
public:
	Source(Id identifier, Vector position, Coordinate radius);
	void release(Molecule *m);
};

#endif /* SIM_SOURCE_H */
