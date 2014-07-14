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

Vector::Vector(const Vector& v) : x(v.x), y(v.y), z(v.z)
{
}

void Vector::translate(const Vector* t)
{
	x += t->x;
	y += t->y;
	z += t->z;
}

std::ostream & operator<<(std::ostream &os, const Vector& v)
{
	static const float _TO_UM = 1000*1000;
	os.precision(2);
	os.width(3);
	os.fill(' ');
	os.width(5);
	return os << "(" << v.x * _TO_UM << "," << v.y * _TO_UM << "," << v.z * _TO_UM << ")um";
}

Vector& Vector::operator=(const Vector& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
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

Vector& Vector::operator-=(const Vector& v)
{
	translate(&v);
	return *this;
}

Vector operator-(Vector lhs, const Vector& rhs)
{
	  lhs -= rhs;
	  return lhs;
}

Vector& Vector::operator/=(const Coordinate d)
{
	this->x /= d;
	this->y /= d;
	this->z /= d;
	return *this;
}

Vector operator/(Vector lhs, Coordinate d)
{
	lhs /= d;
	return lhs;
}

Vector& Vector::operator*=(const Coordinate d)
{
	this->x *= d;
	this->y *= d;
	this->z *= d;
	return *this;
}

Vector operator*(Vector lhs, Coordinate d)
{
	lhs *= d;
	return lhs;
}

Vector operator-(const Vector& v)
{
	return Vector(-v.x, -v.y, -v.z);
}
