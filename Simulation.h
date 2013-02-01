#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <ostream>
#include <vector>

#include "Molecule.h"
#include "Cell.h"

class Simulation
{
	bool sstarted;
	bool sfinished;
	long stime;
public:
	Simulation();
	void run(int nom, int noi, std::vector<Molecule>* molecules, std::vector<Cell>* recv_cells);
	bool running();
	bool started();
	bool finished();
	long time();
};

#endif /* SIM_SIMULATION_H */
