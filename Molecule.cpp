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

void Molecule::move_back()
{
	mposition = mpposition;
}

bool Molecule::check_collision(const Boundary *c)
{
	return segment_line_sphere_intersect(&mposition, &mpposition, c->position(), c->radius());
}

std::ostream & operator<<(std::ostream &os, const Molecule& m)
{
	return os << "Molecule:" << m.mid << " " << (*m.position());
}
