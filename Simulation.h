#ifndef SIM_SIMULATION_H
#define SIM_SIMULATION_H

#include <ostream>
#include <vector>
#include <list>
#include <libconfig.h++>

#include "Molecule.h"
#include "Receptor.h"
#include "Source.h"
#include "Obstacle.h"
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

	std::list<Molecule*>*  smolecules;
	std::vector<Receptor>* sreceivers;
	std::vector<Source>*   stransmitters;
	std::vector<Obstacle>* sobstacles;

	BrownianMotion* bm;
	std::vector<Statistics*> sstat;

	libconfig::Config cfg;

	template<typename BoundaryType>
	std::vector<BoundaryType>* load_configuration(std::string boundary);

	void print_progress();
		
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

	std::list<Molecule*>*  molecules();
	std::vector<Receptor>* receivers();
	std::vector<Source>*   transmitters();
	std::vector<Obstacle>* obstacles();
};

#endif /* SIM_SIMULATION_H */
