#include <vector>
#include "Receptor.h"
#include "Molecule.h"
#include "Types.h"
#include "tri_logger/tri_logger.hpp"

Receptor::Receptor(Id identifier, Vector position, Coordinate radius) :
	Boundary(identifier, position, radius)
{
	rmolecules = new std::vector<Molecule*>();
}

Receptor::Receptor(Id identifier, Vector position, Coordinate size_x, Coordinate size_y, Coordinate size_z) :
	Boundary(identifier, position, size_x, size_y, size_z)
{
	rmolecules = new std::vector<Molecule*>();
}

Receptor::Receptor(const Receptor& other) : Boundary(other), rmolecules( new std::vector<Molecule*>(*(other.rmolecules)) )
{
}

Receptor::~Receptor()
{
	for (std::vector<Molecule*>::iterator mit = rmolecules->begin(); mit != rmolecules->end(); ++mit)
	{
		delete (*mit);
	}
	delete rmolecules;
}

/**
 * return true if collision shall take place and some action be takec accordingly
 */
bool Receptor::collide(Molecule *m)
{
//	TRI_LOG_STR("Collided with molecule " << *m);
	// rmolecules->push_back(m);
	// return true;

	return false;
}

void Receptor::handle_collision(Molecule *m)
{
	// shall take over = set owner.
	// with some probability?
	// m->change_owner(this);
}


std::vector<Molecule*>* Receptor::molecules()
{
	return rmolecules;
}


Receptor& Receptor::operator=(Receptor other)
{
	swap(*this, other);
	return *this;
}

std::ostream & operator<<(std::ostream &os, const Receptor& p)
{
	return os << "Receptor [" << p.id()  << "] of size=" << p.cradius << " at " << p.cposition << ")";
}

// http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
// http://stackoverflow.com/questions/7515617/copy-and-swap-idiom-with-inheritance
void swap(Receptor& first, Receptor& second) // nothrow
{
	TRI_LOG_STR("dbg Receptor::swap .");
	std::swap(first.cposition, second.cposition);
	std::swap(first.cradius, second.cradius);
	std::swap(first.cid, second.cid);
	//std::swap(static_cast<Cell&>(first), static_cast<Cell&>(second));
	TRI_LOG_STR("dbg Receptor::swap ..");
	std::swap(first.rmolecules, second.rmolecules);
	TRI_LOG_STR("dbg Receptor::swap ...");
}
