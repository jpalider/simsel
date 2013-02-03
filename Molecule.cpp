#include <iostream>

#include "Molecule.h"
#include "Cell.h"
#include "Math.h"

using namespace std;

Molecule::Molecule(long identifier, Vector position)
{
	mid = identifier;
	mposition = position;
	mpposition = position;
	mhistogram.insert( std::pair<long,Vector>(0, position) );
	mcell = NULL;
}

const Vector* Molecule::position() const
{
	return &mposition;
}

const map<long, Vector>* Molecule::histogram() const
{
	return &mhistogram;
}


void Molecule::move(long time, Vector t)
{
	mpposition = mposition;
	mposition.translate(&t);
	mhistogram.insert( std::pair<long,Vector>(time, mposition) );
}

bool Molecule::check_collision(Cell *c)
{
	if ( segment_line_sphere_intersect(&mposition, &mpposition, c->position(), c->radius()) )
	{
		c->collide(this);
		this->mcell = c;
	}
	return mcell != NULL;
}

std::ostream & operator<<(std::ostream &os, const Molecule& m)
{
	return os << "Molecule:" << m.mid << " (" << m.position()->x << "," << m.position()->y << "," << m.position()->z << ")";
}
