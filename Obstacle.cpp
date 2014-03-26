#include "Obstacle.h"

Obstacle::Obstacle(Id identifier, Vector position, Coordinate radius) : Boundary(identifier, position, radius)
{
}


bool Obstacle::collide(Molecule *m)
{
	return false;
}
