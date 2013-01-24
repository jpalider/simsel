#include <iostream>
#include "Molecule.h"

using namespace std;

Molecule::Molecule(Position p, float r)
{
	pos = p;
	radius = r;
	histogram.insert( std::pair<long,Position>(0, pos) );
}

void Molecule::print_position()
{
	cout << "Pos(" << pos.x << "," << pos.y << "," << pos.z << ")\n";
}

Position Molecule::get_position()
{
	return pos;
}

float Molecule::get_radius()
{
	return radius;
}


map<long, Position> Molecule::get_histogram()
{
	return histogram;
}


void Molecule::move(long time, Translation t)
{
	pos.translate(t);
	histogram.insert( std::pair<long,Position>(time, pos) );
}
