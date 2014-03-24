#ifndef SIM_RCELL_H
#define SIM_RCELL_H

#include <vector>
#include <memory>

#include "Boundary.h"
#include "Vector.h"
#include "Types.h"

class Molecule;

class RCell : public Boundary
{
private:
	std::vector<Molecule*>* rmolecules ;

public:
	RCell(Id identifier, Vector position, Coordinate radius);
	RCell(const RCell& other);
	~RCell();

	std::vector<Molecule*>* molecules();
	bool collide(Molecule *m);

	RCell& operator=(RCell other);
	friend void swap(RCell& first, RCell& second);
};

#endif /* SIM_RCELL_H */
