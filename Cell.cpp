#include <iostream>

#include "Cell.h"
#include "CairoColor.h"
#include "Vector.h"
#include "Molecule.h"

Cell::Cell(long identifier, Vector position, double radius)
{
	cid = identifier;
	cposition = position;
	cradius = radius;
}

Cell::Cell(const Cell& other)
{
	cid = other.cid;
	cposition = other.cposition;
	cradius = other.cradius;
}


Cell::~Cell()
{
}

double Cell::radius() const
{
	return cradius;
}

const Vector* Cell::position() const
{
	return &cposition;
}


std::ostream & operator<<(std::ostream &os, const Cell& c)
{
	return os << "Cell_" << c.cid << " (" << c.position()->x << "," << c.position()->y << "," << c.position()->z << ")";
}

long Cell::id() const
{
	return cid;
}
