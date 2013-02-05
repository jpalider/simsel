#ifndef SIM_CELL_H
#define SIM_CELL_H

#include <iostream>
#include "Vector.h"

class Cell
{
protected:
	Vector cposition;
	float cradius;
	long cid;

public:
	Cell(long identifier, Vector position, float radius);

	float radius();
	const Vector* position() const;

private:
        friend std::ostream & operator<<(std::ostream &os, const Cell& c);

	
};

#endif /* SIM_CELL_H */













