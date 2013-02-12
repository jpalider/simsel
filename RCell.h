#ifndef SIM_RCELL_H
#define SIM_RCELL_H

#include <vector>

#include "Cell.h"
#include "Vector.h"

class Molecule;

class RCell : public Cell
{
private:
	std::vector<Molecule*>* rmolecules;

public:
	RCell(long identifier, Vector position, double radius);
	std::vector<Molecule*>* molecules();

	bool collide(Molecule *m);
};

#endif /* SIM_RCELL_H */
