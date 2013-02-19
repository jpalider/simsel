#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <ostream>
#include <vector>
#include <list>
#include <libconfig.h++>

#include "Molecule.h"
#include "RCell.h"
#include "TCell.h"
#include "BrownianMotion.h"

class Simulation
{
private:
	bool sstarted;
	bool sfinished;
	long stime;
	long duration;

	std::list<Molecule*>* smolecules;
	std::vector<RCell>* sreceivers;
	std::vector<TCell>* stransmitters;

	BrownianMotion* bm;

	libconfig::Config cfg;

public:
	Simulation();
	~Simulation();
	void run();
	bool running();
	bool started();
	bool finished();
	long time();

	std::list<Molecule*>* molecules();
	std::vector<RCell>* receivers();
	std::vector<TCell>* transmitters();
};

#endif /* SIM_SIMULATION_H */
