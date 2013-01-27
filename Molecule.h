#ifndef SIM_MOLECULE_H
#define SIM_MOLECULE_H
#include <map>

#include "Vector.h"
#include "Translation.h"
#include "CairoColor.h"

class Molecule {
private:
	Vector p;
	float r;
	std::map<long, Vector> h;
	CairoColor c;
public:
	Molecule(Vector position, float radius, CairoColor color);
	float radius();
	Vector position();
	std::map<long, Vector> histogram();
	void move(long time, Translation t);
	void print_position();
	CairoColor* color();
};

#endif /* SIM_MOLECULE_H */
