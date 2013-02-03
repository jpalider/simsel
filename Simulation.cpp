#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <libconfig.h++>

#include "Simulation.h"
#include "Vector.h"
#include "Molecule.h"
#include "CairoColor.h"
#include "tri_logger/tri_logger.hpp"

using namespace std;

Simulation::Simulation()
{
	sstarted = false;
	sfinished = false;
	stime = 0;

	smolecules = new std::vector<Molecule>();
	sreceivers = new std::vector<Cell>();
	stransmitters = new std::vector<Cell>();

	cfg.readFile("cfg/Simulation.cfg");
	string description = cfg.lookup("description");
	TRI_LOG_STR("Simulation:\n" << description << "\n");

	TRI_LOG_STR("Sim: add molecules to the environment");
	int nom = cfg.lookup("simulation.molecules.number");
	Vector p(0, 0, 0);
	for (int i = 0; i < nom; i++)
	{
		smolecules->push_back(Molecule(i, p));
	}

	TRI_LOG_STR("Sim: load receivers configuration");
	
	int cells_no = cfg.lookup("simulation.cells").getLength();
	for (int i = 0; i < cells_no; i++)
	{
		stringstream ss;
		ss << i;
		string prefix = string("simulation.cells.[") + ss.str() + string("].");
		string param_x = prefix + string("pos.x");
		string param_y = prefix + string("pos.y");
		string param_z = prefix + string("pos.z");
		float x = cfg.lookup(param_x);
		float y = cfg.lookup(param_y);
		float z = cfg.lookup(param_z);
		Vector cp(x, y, z);
		int id = cfg.lookup(prefix + string("id"));
		float radius = cfg.lookup(prefix + string("radius"));
		sreceivers->push_back(Cell(id, cp, radius));
		TRI_LOG_STR("cell pos" << cp);
	}

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
			mit->move(stime, bm.get_move(300));
			for (vector<Cell>::iterator cit = sreceivers->begin(); cit != sreceivers->end(); ++cit) {
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

vector<Cell>* Simulation::receivers()
{
	return sreceivers;
}

vector<Cell>* Simulation::transmitters()
{
	return stransmitters;
}
