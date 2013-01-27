#include "iostream"
#include "limits"

#include "Vector.h"

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z)
{
}

Vector::Vector()
{
	x = y = z = std::numeric_limits<float>::infinity();
}

void Vector::translate(Vector* t)
{
	x += t->x;
	y += t->y;
	z += t->z;
}

std::ostream & operator<<(std::ostream &os, const Vector& v)
{
	return os << "(" << v.x << "," << v.y << "," << v.z << ")";
}
