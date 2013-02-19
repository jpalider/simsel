#ifndef SIM_RCELL_H
#define SIM_RCELL_H

#include <vector>
#include <memory>

#include "Cell.h"
#include "Vector.h"

class Molecule;

class RCell : public Cell
{
private:
	std::vector<Molecule*>* rmolecules ;

public:
	RCell(long identifier, Vector position, double radius);
	RCell(const RCell& other);
	~RCell();

	std::vector<Molecule*>* molecules();
	bool collide(Molecule *m);

	RCell& operator=(RCell other);
	friend void swap(RCell& first, RCell& second);
};

#endif /* SIM_RCELL_H */
