#ifndef SIM_CELL_H
#define SIM_CELL_H

#include <iostream>

#include "Vector.h"
#include "CairoColor.h"

class Cell
{
private:
	Vector p;
	float r;
	CairoColor c;
public:
	Cell(Vector position, float radius, CairoColor color);
	float radius();
	const Vector* position() const;
	void move(long time, Vector t);
	CairoColor* color();

private:
        friend std::ostream & operator<<(std::ostream &os, const Cell& c);

	
};

#endif /* SIM_CELL_H */













