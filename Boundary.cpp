#include <iostream>

#include "Boundary.h"
#include "CairoColor.h"
#include "Vector.h"
#include "Molecule.h"

Boundary::Boundary(long identifier, Vector position, double radius)
{
	cid = identifier;
	cposition = position;
	cradius = radius;
}

Boundary::Boundary(const Boundary& other)
{
	cid = other.cid;
	cposition = other.cposition;
	cradius = other.cradius;
}


Boundary::~Boundary()
{
}

double Boundary::radius() const
{
	return cradius;
}

const Vector* Boundary::position() const
{
	return &cposition;
}


std::ostream & operator<<(std::ostream &os, const Boundary& c)
{
	return os << "Boundary_" << c.cid << " (" << c.position()->x << "," << c.position()->y << "," << c.position()->z << ")";
}

long Boundary::id() const
{
	return cid;
}
