#include <iostream>
#include "Molecule.h"

using namespace std;

Molecule::Molecule(Vector position, float radius, CairoColor color)
{
	p = position;
	r = radius;
	h.insert( std::pair<long,Vector>(0, p) );
	c = color;
}

void Molecule::print_position()
{
	cout << "Pos(" << p.x << "," << p.y << "," << p.z << ")\n";
}

Vector Molecule::position()
{
	return p;
}

float Molecule::radius()
{
	return r;
}


map<long, Vector> Molecule::histogram()
{
	return h;
}


void Molecule::move(long time, Translation t)
{
	p.translate(t);
	h.insert( std::pair<long,Vector>(time, p) );
}

CairoColor* Molecule::color()
{
	return &c;
}
