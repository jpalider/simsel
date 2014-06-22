#ifndef SIM_VECTOR_H
#define SIM_VECTOR_H

#include <ostream>
#include "Types.h"

class Vector {
public:	
	Coordinate x, y, z;

	Vector(Coordinate x, Coordinate y, Coordinate z);
	Vector();

	void translate(const Vector* t);

	Vector& operator+=(const Vector& v);
	Vector& operator-=(const Vector& v);
	Vector& operator/=(const Coordinate);
	Vector& operator*=(const Coordinate);

private:
	friend Vector operator*(Vector lhs, const Coordinate d);
	friend Vector operator/(Vector lhs, const Coordinate d);
	friend Vector operator+(Vector lhs, const Vector& rhs);	
	friend Vector operator-(Vector lhs, const Vector& rhs);
	friend Vector operator-(const Vector& v);
	friend std::ostream & operator<<(std::ostream &os, const Vector& p);
};

#endif /* SIM_VECTOR_H */
