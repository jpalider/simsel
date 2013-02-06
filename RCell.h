#ifndef SIM_RCELL_H
#define SIM_RCELL_H

#include <vector>

#include "Cell.h"
#include "Molecule.h"

class Molecule;

class RCell : public Cell
{
private:
	std::vector<Molecule*>* rmolecules;
public:
	RCell(long identifier, Vector position, float radius);
	void collide(Molecule *m);
	std::vector<Molecule*>* molecules();
};

#endif /* SIM_RCELL_H */
