#include <vector>
#include "RCell.h"
#include "Molecule.h"
#include "tri_logger/tri_logger.hpp"

RCell::RCell(long identifier, Vector position, double radius) :
	Cell(identifier, position, radius)
{
	rmolecules = new std::vector<Molecule*>();
}

bool RCell::collide(Molecule *m)
{
	TRI_LOG_STR("Collided with molecule " << *m);
	rmolecules->push_back(m);
	return true;
}

std::vector<Molecule*>* RCell::molecules()
{
	return rmolecules;
}



