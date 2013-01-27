#include "iostream"
#include "limits"

#include "Vector.h"
#include "Translation.h"

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z)
{
}

Vector::Vector()
{
	x = y = z = std::numeric_limits<float>::infinity();
}

void Vector::translate(Translation t)
{
	x += t.x;
	y += t.y;
	z += t.z;
}

std::ostream & operator<<(std::ostream &os, const Vector& p)
{
	return os << "(" << p.x << "," << p.y << "," << p.z << ")";
}
