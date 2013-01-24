#include "iostream"
#include "limits"

#include "Position.h"
#include "Translation.h"

Position::Position(float x, float y, float z) : x(x), y(y), z(z)
{
}

Position::Position()
{
	x = y = z = std::numeric_limits<float>::infinity();
}

void Position::translate(Translation t)
{
	x += t.x;
	y += t.y;
	z += t.z;
}
