#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <ostream>
#include <vector>
#include <libconfig.h++>

#include "Molecule.h"
#include "Cell.h"
#include "BrownianMotion.h"

class Simulation
{
private:
	bool sstarted;
	bool sfinished;
	long stime;
	long duration;
	
	std::vector<Molecule>* smolecules;
	std::vector<Cell>* sreceivers;
	std::vector<Cell>* stransmitters;

	BrownianMotion bm;
//	Vector p(0, 0, 0);

	libconfig::Config cfg;

public:
	Simulation();
	void run();
	bool running();
	bool started();
	bool finished();
	long time();

	std::vector<Molecule>* molecules();
	std::vector<Cell>* receivers();	
	std::vector<Cell>* transmitters();
};

#endif /* SIM_SIMULATION_H */
