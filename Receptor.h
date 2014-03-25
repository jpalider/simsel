#ifndef SIM_RECEPTOR_H
#define SIM_RECEPTOR_H

#include <vector>
#include <memory>

#include "Boundary.h"
#include "Vector.h"
#include "Types.h"

class Molecule;

class Receptor : public Boundary
{
private:
	std::vector<Molecule*>* rmolecules ;

public:
	Receptor(Id identifier, Vector position, Coordinate radius);
	Receptor(const Receptor& other);
	~Receptor();

	std::vector<Molecule*>* molecules();
	bool collide(Molecule *m);

	Receptor& operator=(Receptor other);
	friend void swap(Receptor& first, Receptor& second);
};

#endif /* SIM_RECEPTOR_H */
