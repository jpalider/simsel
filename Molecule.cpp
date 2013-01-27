#include <iostream>
#include "Molecule.h"

using namespace std;

Molecule::Molecule(Vector position, float radius, CairoColor color)
{
	p = position;
	r = radius;
	h.insert( std::pair<long,Vector>(0, p) );
	c = color;
}

const Vector* Molecule::position() const
{
	return &p;
}

const map<long, Vector>* Molecule::histogram() const
{
	return &h;
}


void Molecule::move(long time, Vector t)
{
	p.translate(&t);
	h.insert( std::pair<long,Vector>(time, p) );
}

CairoColor* Molecule::color()
{
	return &c;
}

std::ostream & operator<<(std::ostream &os, const Molecule& m)
{
	return os << "(" << m.position()->x << "," << m.position()->y << "," << m.position()->z << ")";
}
