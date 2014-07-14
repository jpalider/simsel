#include <iostream>

#include "Molecule.h"
#include "Boundary.h"
#include "Math.h"

using namespace std;

Molecule::Molecule(Id identifier, Vector position, void *owner)
{
	mid = identifier;
	mposition = position;
	mpposition = position;
	mcell = nullptr;
	mowner = owner;
}

const Vector* Molecule::position() const
{
	return &mposition;
}

const Vector* Molecule::prev_position() const
{
	return &mpposition;
}

void Molecule::move(Vector t)
{
	mpposition = mposition;
	mposition += t;
}

void Molecule::move_back()
{
	mposition = mpposition;
}

bool Molecule::is_owner(void * owner)
{
	return mowner == owner;
}

void Molecule::change_owner(void * owner)
{
	mowner = owner;
}

std::ostream & operator<<(std::ostream &os, const Molecule& m)
{
	return os << "Molecule:" << m.mid << " " << (*m.position());
}
