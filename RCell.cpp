#include <vector>
#include "RCell.h"
#include "Molecule.h"
#include "tri_logger/tri_logger.hpp"

RCell::RCell(long identifier, Vector position, double radius) :
	Cell(identifier, position, radius)
{
	rmolecules = new std::vector<Molecule*>();
}

RCell::RCell(const RCell& other) : Cell(other),	rmolecules( new std::vector<Molecule*>(*(other.rmolecules)) )
{
}


RCell::~RCell()
{
	for (std::vector<Molecule*>::iterator mit = rmolecules->begin(); mit != rmolecules->end(); ++mit)
	{
		delete (*mit);
	}
	delete rmolecules;
}

/**
 * return true if collision shall take place, and collision results in copying Molecule
 */
bool RCell::collide(Molecule *m)
{
//	TRI_LOG_STR("Collided with molecule " << *m);
	rmolecules->push_back(m);
	return true;
}

std::vector<Molecule*>* RCell::molecules()
{
	return rmolecules;
}


RCell& RCell::operator=(RCell other)
{
	swap(*this, other);
	return *this;
}

// http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
// http://stackoverflow.com/questions/7515617/copy-and-swap-idiom-with-inheritance
void swap(RCell& first, RCell& second) // nothrow
{
	TRI_LOG_STR("dbg RCell::swap .");
	std::swap(first.cposition, second.cposition);
	std::swap(first.cradius, second.cradius);
	std::swap(first.cid, second.cid);
	//std::swap(static_cast<Cell&>(first), static_cast<Cell&>(second));
	TRI_LOG_STR("dbg RCell::swap ..");
	std::swap(first.rmolecules, second.rmolecules);
	TRI_LOG_STR("dbg RCell::swap ...");
}
