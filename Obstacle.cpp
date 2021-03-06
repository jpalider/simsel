#include "Obstacle.h"

Obstacle::Obstacle(Id identifier, Vector position, Coordinate radius) : Boundary(identifier, position, radius)
{
}

Obstacle::Obstacle(Id identifier, Vector position, Coordinate size_x, Coordinate size_y, Coordinate size_z) : Boundary(identifier, position, size_x, size_y, size_z)
{
}

bool Obstacle::collide(Molecule *m)
{
	return true;
}

void Obstacle::handle_collision(Molecule *m)
{
}

std::ostream & operator<<(std::ostream &os, const Obstacle& c)
{
	return os << "Obstacle_" << c.cid << " " << c.cposition << " " << c.cradius << " " << c.corner_b << c.corner_e;
}

