#include "TCell.h"
#include "Molecule.h"
#include "tri_logger/tri_logger.hpp"

TCell::TCell(long identifier, Vector position, float radius) :
	Cell(identifier, position, radius)
{
}

