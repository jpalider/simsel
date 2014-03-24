#ifndef SIM_BOUNDARY_H
#define SIM_BOUNDARY_H

#include <iostream>

#include "Vector.h"

class Molecule;

class Boundary
{
protected:
	Vector cposition;
	double cradius;
	long cid;

public:
	Boundary(long identifier, Vector position, double radius);
	Boundary(const Boundary &other);

	virtual ~Boundary();
	double radius() const;
	const Vector* position() const;
	long id() const;

	virtual bool collide(Molecule *m) = 0;

private:
        friend std::ostream & operator<<(std::ostream &os, const Boundary& c);

	
};

#endif /* SIM_BOUNDARY_H */













