#ifndef SIM_MOLECULE_H
#define SIM_MOLECULE_H

#include <map>
#include <iostream>

#include "Vector.h"
#include "RCell.h"

class RCell;

class Molecule {
private:
	Vector mposition;
	Vector mpposition;
	float mradius;
	std::map<long, Vector> mhistogram;
	long mid;
	RCell* mcell;

public:
	Molecule(long identifier, Vector position);

	const Vector* position() const;
	const std::map<long, Vector>* histogram() const;

	void move(long time, Vector t);
	bool check_collision(RCell *c);

private:
	friend std::ostream & operator<<(std::ostream &os, const Molecule& m);

};

#endif /* SIM_MOLECULE_H */
