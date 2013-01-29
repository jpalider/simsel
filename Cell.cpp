#include <iostream>

#include "Cell.h"
#include "CairoColor.h"
#include "Vector.h"
#include "Molecule.h"

Cell::Cell(long identifier, Vector position, float radius, CairoColor color)
{
	id = identifier;
	p = position;
	c = color;
	r = radius;
}

float Cell::radius()
{
	return r;
}

const Vector* Cell::position() const
{
	return &p;
}

CairoColor* Cell::color()
{
	return &c;
}

void Cell::collide(Molecule *m)
{
	std::cout << "Collided with molecule " << *m << std::endl;
}


std::ostream & operator<<(std::ostream &os, const Cell& c)
{
	return os << "(" << c.position()->x << "," << c.position()->y << "," << c.position()->z << ")";
}
