#include <iostream>

#include "Molecule.h"
#include "Boundary.h"
#include "Math.h"

using namespace std;

Molecule::Molecule(Id identifier, Vector position)
{
	mid = identifier;
	mposition = position;
	mpposition = position;
	mcell = NULL;
}

const Vector* Molecule::position() const
{
	return &mposition;
}

void Molecule::move(Vector t)
{
	mpposition = mposition;
	mposition.translate(&t);
}

bool Molecule::check_collision(Boundary *c)
{
	bool received = false;
	if ( segment_line_sphere_intersect(&mposition, &mpposition, c->position(), c->radius()) )
	{
		received = c->collide(this);
		this->mcell = c;
	}
	return received;
}

std::ostream & operator<<(std::ostream &os, const Molecule& m)
{
	return os << "Molecule:" << m.mid << " " << (*m.position());
}
