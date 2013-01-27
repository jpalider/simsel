#include <iostream>

#include "Cell.h"
#include "CairoColor.h"
#include "Vector.h"

std::ostream & operator<<(std::ostream &os, const Cell& c)
{
	return os << "(" << c.position()->x << "," << c.position()->y << "," << c.position()->z << ")";
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

