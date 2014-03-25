#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <ostream>
#include <vector>
#include <list>
#include <libconfig.h++>

#include "Molecule.h"
#include "Receptor.h"
#include "Source.h"
#include "BrownianMotion.h"
#include "Statistics.h"

class Simulation
{
private:
	double ssim_scale;

	bool sstarted;
	bool sfinished;
	long stime;
	long duration;
	long stime_step;

	std::list<Molecule*>* smolecules;
	std::vector<Receptor>* sreceivers;
	std::vector<Source>* stransmitters;

	BrownianMotion* bm;
	std::vector<Statistics*> sstat;

	libconfig::Config cfg;

public:
	Simulation();
	~Simulation();
	void run();
	bool running();
	bool started();
	bool finished();
	long time();
	long interval();

	double scale();

	void add(Statistics* statistics);
	// void add(Player* player); // one day maybe

	std::list<Molecule*>* molecules();
	std::vector<Receptor>* receivers();
	std::vector<Source>* transmitters();
};

#endif /* SIM_SIMULATION_H */
