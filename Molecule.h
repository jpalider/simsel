#ifndef SIM_MOLECULE_H
#define SIM_MOLECULE_H

#include <map>
#include <iostream>

#include "Vector.h"
#include "CairoColor.h"
#include "Cell.h"

class Cell;

class Molecule {
private:
	Vector p;
	Vector pp;
	float r;
	std::map<long, Vector> h;
	CairoColor c;
	long id;
public:
	Molecule(long identifier, Vector position, CairoColor color);

	const Vector* position() const;
	const std::map<long, Vector>* histogram() const;
	CairoColor* color();

	void move(long time, Vector t);
	void check_collision(Cell *c);
		
private:
	friend std::ostream & operator<<(std::ostream &os, const Molecule& m);

};

#endif /* SIM_MOLECULE_H */
