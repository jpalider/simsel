#include "Source.h"
#include "Molecule.h"
#include "tri_logger/tri_logger.hpp"

Source::Source(Id identifier, Vector position, Coordinate radius) : Boundary(identifier, position, radius)
{
}

void Source::release(Molecule *m)
{

}

bool Source::collide(Molecule *m)
{
	return false;
}
