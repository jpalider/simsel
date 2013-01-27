#ifndef SIM_MOLECULE_H
#define SIM_MOLECULE_H

#include <map>
#include <iostream>

#include "Vector.h"
#include "CairoColor.h"

class Molecule {
private:
	Vector p;
	float r;
	std::map<long, Vector> h;
	CairoColor c;
public:
	Molecule(Vector position, float radius, CairoColor color);

	const Vector* position() const;
	const std::map<long, Vector>* histogram() const;
	CairoColor* color();

	void move(long time, Vector t);
		
private:
	friend std::ostream & operator<<(std::ostream &os, const Molecule& m);

};

#endif /* SIM_MOLECULE_H */
