#ifndef SIM_BOUNDARY_H
#define SIM_BOUNDARY_H

#include <iostream>

#include "Vector.h"
#include "Types.h"

class Molecule;

class Boundary
{
protected:
	Vector cposition;
	Coordinate cradius;
	Id cid;
public:
	Boundary(Id identifier, Vector position, Coordinate radius);
	Boundary(const Boundary &other);

	virtual ~Boundary();
	Coordinate radius() const;
	const Vector* position() const;
	Id id() const;

	/** Returns true if a given on non-transparent or shall act on molecule */
	virtual bool collide(Molecule *m) = 0;
	virtual void handle_collision(Molecule *m) = 0;

private:
        friend std::ostream & operator<<(std::ostream &os, const Boundary& c);

	
};

#endif /* SIM_BOUNDARY_H */













