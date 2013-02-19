#include "TCell.h"
#include "Molecule.h"
#include "tri_logger/tri_logger.hpp"

TCell::TCell(long identifier, Vector position, double radius) :
	Cell(identifier, position, radius)
{
}

TCell::TCell(const TCell& other) : Cell(other)
{

}

bool TCell::collide(Molecule *m)
{
	return false;
}
