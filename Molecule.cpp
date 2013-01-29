#include <iostream>

#include "Molecule.h"
#include "Cell.h"
#include "Math.h"

using namespace std;

Molecule::Molecule(long identifier, Vector position, CairoColor color)
{
	id = identifier;
	p = position;
	pp = position;
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
	pp = p;
	p.translate(&t);
	h.insert( std::pair<long,Vector>(time, p) );
}

void Molecule::check_collision(Cell *c)
{
	if ( segment_line_sphere_intersect(&p, &pp, c->position(), c->radius()) )
	{
		c->collide(this);
		this->c = CairoColor(0.f,0.f,0.f);
	}
}


CairoColor* Molecule::color()
{
	return &c;
}

std::ostream & operator<<(std::ostream &os, const Molecule& m)
{
	return os << m.id << ":(" << m.position()->x << "," << m.position()->y << "," << m.position()->z << ")";
}
