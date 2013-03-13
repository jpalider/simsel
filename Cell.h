#ifndef SIM_CELL_H
#define SIM_CELL_H

#include <iostream>

#include "Vector.h"

class Molecule;

class Cell
{
protected:
	Vector cposition;
	double cradius;
	long cid;

public:
	Cell(long identifier, Vector position, double radius);
	Cell(const Cell &other);

	virtual ~Cell();
	double radius() const;
	const Vector* position() const;
	long id() const;

	virtual bool collide(Molecule *m) = 0;

private:
        friend std::ostream & operator<<(std::ostream &os, const Cell& c);

	
};

#endif /* SIM_CELL_H */













