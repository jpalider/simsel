#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <libconfig.h++>
#include <ctime>
#include <cstdlib>


#include "Simulation.h"
#include "Vector.h"
#include "Molecule.h"
#include "CairoColor.h"
#include "RCell.h"
#include "TCell.h"
#include "tri_logger/tri_logger.hpp"

using namespace std;

Simulation::Simulation()
{
	sstarted = false;
	sfinished = false;
	stime = 0;

	smolecules = new std::vector<Molecule>();
	sreceivers = new std::vector<RCell>();
	stransmitters = new std::vector<TCell>();

	cfg.readFile("cfg/Simulation.cfg");
	string description = cfg.lookup("description");
	TRI_LOG_STR("Simulation:\n" << description);

	bool repetitive = cfg.lookup("simulation.repetitive");
	long seed = repetitive ? 1 : std::time(NULL);
	srand(seed);

	TRI_LOG_STR("Sim: add molecules to the environment");
	int nom = cfg.lookup("simulation.molecules.number");
	Vector p(0, 0, 0);
	for (int i = 0; i < nom; i++)
	{
		smolecules->push_back(Molecule(i, p));
	}

	int cells_no = 0;
	TRI_LOG_STR("Sim: load receivers configuration");
	cells_no = cfg.lookup("simulation.rcells").getLength();
	for (int i = 0; i < cells_no; i++)
	{
		stringstream ss;
		ss << i;
		string prefix = string("simulation.rcells.[") + ss.str() + string("].");
		string param_x = prefix + string("pos.x");
		string param_y = prefix + string("pos.y");
		string param_z = prefix + string("pos.z");
		float x = cfg.lookup(param_x);
		float y = cfg.lookup(param_y);
		float z = cfg.lookup(param_z);
		Vector cp(x, y, z);
		int id = cfg.lookup(prefix + string("id"));
		float radius = cfg.lookup(prefix + string("radius"));
		sreceivers->push_back(RCell(id, cp, radius));
		TRI_LOG_STR("rcell pos" << cp);
	}

	TRI_LOG_STR("Sim: load transmitters configuration");
	cells_no = cfg.lookup("simulation.tcells").getLength();
	for (int i = 0; i < cells_no; i++)
	{
		stringstream ss;
		ss << i;
		string prefix = string("simulation.tcells.[") + ss.str() + string("].");
		string param_x = prefix + string("pos.x");
		string param_y = prefix + string("pos.y");
		string param_z = prefix + string("pos.z");
		float x = cfg.lookup(param_x);
		float y = cfg.lookup(param_y);
		float z = cfg.lookup(param_z);
		Vector cp(x, y, z);
		int id = cfg.lookup(prefix + string("id"));
		float radius = cfg.lookup(prefix + string("radius"));
		stransmitters->push_back(TCell(id, cp, radius));
		TRI_LOG_STR("tcell pos" << cp);
	}

	TRI_LOG_STR("Sim: load duration");
	duration = cfg.lookup("simulation.duration");
}

void Simulation::run()
{
	sstarted = true;


	TRI_LOG_STR("Starting simulation");
	// for (int i = 0; i < duration; i++) 
	while (stime < duration)
	{
		stime += 1;
		
		// for all molecules perform their action
		for (vector<Molecule>::iterator mit = smolecules->begin(); mit != smolecules->end(); ++mit) {
			mit->move(stime, bm.get_move(10));
			for (vector<RCell>::iterator cit = sreceivers->begin(); cit != sreceivers->end(); ++cit) {
				mit->check_collision(&(*cit));
			}
		}
	}
	sfinished = true;
	TRI_LOG_STR("Finished simulation");
	
}

bool Simulation::running()
{
	return sstarted && !sfinished;
}

bool Simulation::started()
{
	return sstarted;
}

bool Simulation::finished()
{
	return sfinished;
}

long Simulation::time()
{
	return stime;
}

vector<Molecule>* Simulation::molecules()
{
	return smolecules;
}

vector<RCell>* Simulation::receivers()
{
	return sreceivers;
}

vector<TCell>* Simulation::transmitters()
{
	return stransmitters;
}
