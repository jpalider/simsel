#include "TCell.h"
#include "Molecule.h"
#include "tri_logger/tri_logger.hpp"

TCell::TCell(Id identifier, Vector position, Coordinate radius) :
	Boundary(identifier, position, radius)
{
}

TCell::TCell(const TCell& other) : Boundary(other)
{

}

bool TCell::collide(Molecule *m)
{
	return false;
}
