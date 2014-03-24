#ifndef SIM_TCELL_H
#define SIM_TCELL_H

#include "Boundary.h"

class Molecule;

class TCell : public Boundary
{
public:
	TCell(Id identifier, Vector position, Coordinate radius);
	TCell(const TCell& other);
	bool collide(Molecule *m);
};

#endif /* SIM_TCELL_H */
