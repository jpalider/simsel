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
	Vector mposition;
	Vector mpposition;
	float mradius;
	std::map<long, Vector> mhistogram;
	long mid;
	Cell* mcell;
public:
	Molecule(long identifier, Vector position);

	const Vector* position() const;
	const std::map<long, Vector>* histogram() const;

	void move(long time, Vector t);
	void check_collision(Cell *c);
		
private:
	friend std::ostream & operator<<(std::ostream &os, const Molecule& m);

};

#endif /* SIM_MOLECULE_H */
