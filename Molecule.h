#ifndef SIM_MOLECULE_H
#define SIM_MOLECULE_H
#include <map>

#include "Position.h"
#include "Translation.h"

class Molecule {
private:
	Position pos;
	float radius;
	std::map<long, Position> histogram;
public:
	Molecule(Position p, float r);
	float get_radius();
	Position get_position();
	std::map<long, Position> get_histogram();
	void move(long time, Translation t);
	void print_position();	
};

#endif /* SIM_MOLECULE_H */
