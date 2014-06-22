#include <iostream>

#include "Boundary.h"
#include "CairoColor.h"
#include "Vector.h"
#include "Molecule.h"
#include "Types.h"
#include "Math.h"

Boundary::Boundary(Id identifier, Vector position, Coordinate radius)
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

Coordinate Boundary::radius() const
{
	return cradius;
}

const Vector* Boundary::position() const
{
	return &cposition;
}


bool Boundary::check_collision(const Molecule *m)
{
	return segment_line_sphere_intersect(m->position(), m->prev_position(), &cposition, cradius);
}

bool Boundary::has_inside(Molecule *m)
{
	return squared_distance_between_points(position(), m->position()) < cradius*cradius;
}


std::ostream & operator<<(std::ostream &os, const Boundary& c)
{
	return os << "Boundary_" << c.cid << " (" << c.position()->x << "," << c.position()->y << "," << c.position()->z << ")";
}

Id Boundary::id() const
{
	return cid;
}
