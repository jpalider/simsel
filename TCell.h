#ifndef SIM_TCELL_H
#define SIM_TCELL_H

#include "Cell.h"

class Molecule;

class TCell : public Cell
{
public:
	TCell(long identifier, Vector position, double radius);
	TCell(const TCell& other);
	bool collide(Molecule *m);
};

#endif /* SIM_TCELL_H */
