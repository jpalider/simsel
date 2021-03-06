#ifndef SIM_RECEPTOR_H
#define SIM_RECEPTOR_H

#include <vector>
#include <memory>

#include "Boundary.h"
#include "Vector.h"
#include "Types.h"

class Molecule;

/*
  Receptor object is transparent to the molecules, it serves as a volume for statistics
 */
class Receptor : public Boundary
{
private:
	std::vector<Molecule*>* rmolecules ;

public:
	Receptor(Id identifier, Vector position, Coordinate radius);
	Receptor(Id identifier, Vector position, Coordinate size_x, Coordinate size_y, Coordinate size_z);
	Receptor(const Receptor& other);
	~Receptor();

	std::vector<Molecule*>* molecules();
	bool collide(Molecule *m);
	void handle_collision(Molecule *m);

	Receptor& operator=(Receptor other);
	friend std::ostream & operator<<(std::ostream &os, const Receptor& p);
	friend void swap(Receptor& first, Receptor& second);
};

#endif /* SIM_RECEPTOR_H */
