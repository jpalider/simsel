#include <iostream>
#include <limits>
#include "Vector.h"

Vector::Vector(Coordinate x, Coordinate y, Coordinate z) : x(x), y(y), z(z)
{
}

Vector::Vector()
{
	x = y = z = std::numeric_limits<Coordinate>::infinity();
}

void Vector::translate(const Vector* t)
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
	translate(&v);
	return *this;
}

Vector operator+(Vector lhs, const Vector& rhs)
{
	  lhs += rhs;
	  return lhs;
}

Vector& Vector::operator/=(const float d)
{
	this->x /= d;
	this->y /= d;
	this->z /= d;
	return *this;
}

Vector operator/(Vector lhs, float d)
{
	lhs /= d;
	return lhs;
}
