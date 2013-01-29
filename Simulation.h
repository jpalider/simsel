#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <ostream>
#include <vector>

#include "Molecule.h"
#include "Cell.h"

class Simulation
{
public:
	static void simulate(int nom, int noi, std::vector<Molecule>* molecules, std::vector<Cell>* recv_cells);
};

#endif /* SIM_SIMULATION_H */
