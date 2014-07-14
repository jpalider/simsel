#include <iostream>

#include "Boundary.h"
#include "CairoColor.h"
#include "Vector.h"
#include "Molecule.h"
#include "Types.h"
#include "Math.h"

BoxCollider    ColliderFactory::box_collider    = BoxCollider();
SphereCollider ColliderFactory::sphere_collider = SphereCollider();

Boundary::Boundary(Id identifier, Vector position, Coordinate radius)
{
	cid = identifier;
	cposition = position;
	cradius = radius;
	collider = ColliderFactory::sphere();
}

Boundary::Boundary(Id identifier, Vector position, Coordinate size_x, Coordinate size_y, Coordinate size_z)
{
	cid = identifier;
	cposition = position;
	xsize = size_x;
	ysize = size_y;
	zsize = size_z;
	collider = ColliderFactory::box();
	corner_b = Vector(position.x - size_x/2, position.y - size_y/2, position.z - size_z/2);
	corner_e = Vector(position.x + size_x/2, position.y + size_y/2, position.z + size_z/2);
}

Boundary::Boundary(const Boundary& other)
{
	cid = other.cid;
	cposition = other.cposition;
	cradius = other.cradius;
	xsize = other.xsize;
	ysize = other.ysize;
	zsize = other.zsize;
	collider = other.collider;
	corner_b = other.corner_b;
	corner_e = other.corner_e;
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

const Vector* Boundary::corner_begin() const
{
	return &corner_b;
}

const Vector* Boundary::corner_end() const
{
	return &corner_e;
}

bool Boundary::check_collision(const Molecule *m) const
{
	return collider->check_collision(m, this);
}

bool Boundary::has_inside(Molecule *m) const
{
	return collider->has_inside(m, this);
}


std::ostream & operator<<(std::ostream &os, const Boundary& c)
{
	return os << "Boundary_" << c.cid << " " << c.cposition << " " << c.cradius << " " << c.corner_b << c.corner_e;
}

Id Boundary::id() const
{
	return cid;
}
