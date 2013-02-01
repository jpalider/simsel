#ifndef SIM_CELL_H
#define SIM_CELL_H

#include <iostream>
#include <vector>

#include "Vector.h"
#include "CairoColor.h"
#include "Molecule.h"

class Molecule;

class Cell
{
private:
	Vector cposition;
	float cradius;
	long cid;
	std::vector<Molecule*>* cmolecules;

public:
	Cell(long identifier, Vector position, float radius);

	float radius();
	const Vector* position() const;

	void collide(Molecule *m);

private:
        friend std::ostream & operator<<(std::ostream &os, const Cell& c);

	
};

#endif /* SIM_CELL_H */













