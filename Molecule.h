#ifndef SIM_MOLECULE_H
#define SIM_MOLECULE_H

#include <map>
#include <iostream>

#include "Types.h"
#include "Vector.h"
#include "Boundary.h"

class RCell;

class Molecule {
private:
	Vector                 mposition;
	Vector                 mpposition;
	Coordinate             mradius;
	Id                     mid;
	Boundary*              mcell;

public:
	Molecule(Id identifier, Vector position);

	const Vector* position() const;

	void move(Vector t);
	bool check_collision(Boundary *c);

private:
	friend std::ostream & operator<<(std::ostream &os, const Molecule& m);

};

#endif /* SIM_MOLECULE_H */
