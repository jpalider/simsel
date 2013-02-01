#include <iostream>

#include "Cell.h"
#include "CairoColor.h"
#include "Vector.h"
#include "Molecule.h"

Cell::Cell(long identifier, Vector position, float radius)
{
	cid = identifier;
	cposition = position;
	cradius = radius;
	cmolecules = new std::vector<Molecule*>();
}

float Cell::radius()
{
	return cradius;
}

const Vector* Cell::position() const
{
	return &cposition;
}

void Cell::collide(Molecule *m)
{
	std::cout << "Collided with molecule " << *m << std::endl;
	cmolecules->push_back(m);
}


std::ostream & operator<<(std::ostream &os, const Cell& c)
{
	return os << "Cell_" << c.cid << " (" << c.position()->x << "," << c.position()->y << "," << c.position()->z << ")";
}
