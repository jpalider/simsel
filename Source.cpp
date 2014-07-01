#include "Source.h"
#include "Molecule.h"
#include "tri_logger/tri_logger.hpp"

Source::Source(Id identifier, Vector position, Coordinate radius) : Boundary(identifier, position, radius)
{
}

Source::Source(Id identifier, Vector position, Coordinate size_x, Coordinate size_y, Coordinate size_z) : Boundary(identifier, position, size_x, size_y, size_z)
{
}

void Source::release(Molecule *m)
{

}

bool Source::collide(Molecule *m)
{
	return false;
}

void Source::handle_collision(Molecule *m)
{

}
