#include "Source.h"
#include "Molecule.h"
#include "tri_logger/tri_logger.hpp"

Source::Source(Id identifier, Vector position, Coordinate radius) : Boundary(identifier, position, radius)
{
}

Source::Source(Id identifier, Vector position, Coordinate size_x, Coordinate size_y, Coordinate size_z) : Boundary(identifier, position, size_x, size_y, size_z)
{
}

bool Source::release(long time, MStore *molecules, Obstacle *space)
{
	static bool done = false;
	if (time > 500000 && !done)
	{
		done = true;
		for (int i = 0; i < 100000; i++)
		{
			molecules->push_back(Molecule(i, cposition, space));
		}
		return true;
	}
	return false;
}

bool Source::collide(Molecule *m)
{
	return false;
}

void Source::handle_collision(Molecule *m)
{

}
