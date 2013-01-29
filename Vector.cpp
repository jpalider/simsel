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

Vector& Vector::operator+=(const Vector& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

inline Vector operator+(Vector lhs, const Vector& rhs)
{
	  lhs += rhs;
	  return lhs;
}
