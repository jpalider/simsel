#ifndef SIM_CELL_H
#define SIM_CELL_H

#include <iostream>

#include "Vector.h"
#include "CairoColor.h"
#include "Molecule.h"

class Molecule;

class Cell
{
private:
	Vector p;
	float r;
	CairoColor c;
	long id;
public:
	Cell(long identifier, Vector position, float radius, CairoColor color);

	float radius();
	const Vector* position() const;
	CairoColor* color();

	void collide(Molecule *m);

private:
        friend std::ostream & operator<<(std::ostream &os, const Cell& c);

	
};

#endif /* SIM_CELL_H */













