#ifndef SIM_VECTOR_H
#define SIM_VECTOR_H

#include <ostream>
#include "Types.h"

class Vector {
public:	
	Coordinate x, y, z;

	Vector(Coordinate x, Coordinate y, Coordinate z);
	Vector();

	void translate(Vector* t);

	Vector& operator+=(const Vector& v);

private:
        friend std::ostream & operator<<(std::ostream &os, const Vector& p);
};

#endif /* SIM_VECTOR_H */
