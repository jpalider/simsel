#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <libconfig.h++>
#include <ctime>
#include <cstdlib>
#include <cmath>

#include "Simulation.h"
#include "Vector.h"
#include "Molecule.h"
#include "CairoColor.h"
#include "RCell.h"
#include "TCell.h"
#include "Math.h"
#include "tri_logger/tri_logger.hpp"

using namespace std;

static double sim_scale = 1e-9;

Simulation::Simulation()
{
	sstarted = false;
	sfinished = false;
	stime = 0;

	smolecules = new std::list<Molecule*>();
	sreceivers = new std::vector<RCell>();
	stransmitters = new std::vector<TCell>();

	cfg.readFile("cfg/Simulation.cfg");
	string description = cfg.lookup("description");
	TRI_LOG_STR("Simulation:\n" << description);

	bool repetitive = cfg.lookup("simulation.repetitive");
	int seed = repetitive ? cfg.lookup("simulation.seed") : std::time(NULL);
	srand(seed);
	TRI_LOG_STR("Repetitivity set to: " << (repetitive ? "true" : "false") << " with seed: " << seed);

	int dimensions = cfg.lookup("simulation.dimensions");
	TRI_LOG_STR("Brownian motion diemnsions set to: " << dimensions);

	int time_step = cfg.lookup("simulation.time_step_ns");
	double tau = time_step * 1e-9;
	TRI_LOG_STR("Brownian motion time step set to: " << tau);

	bm = new BrownianMotion(dimensions, tau);

	int nom = cfg.lookup("simulation.molecules.number");
	Vector p(0, 0, 0);
	for (int i = 0; i < nom; i++)
	{
		smolecules->push_back(new Molecule(i, p));
	}
	TRI_LOG_STR("Number of molecules added to the environment: " << nom);


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
		double x = cfg.lookup(param_x);
		double y = cfg.lookup(param_y);
		double z = cfg.lookup(param_z);
		x *= sim_scale;
		y *= sim_scale;
		z *= sim_scale;
		Vector cp(x, y, z);
		int id = cfg.lookup(prefix + string("id"));
		double radius = cfg.lookup(prefix + string("radius"));
		radius *= sim_scale;
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
		double x = cfg.lookup(param_x);
		double y = cfg.lookup(param_y);
		double z = cfg.lookup(param_z);
		x *= sim_scale;
		y *= sim_scale;
		z *= sim_scale;
		Vector cp(x, y, z);
		int id = cfg.lookup(prefix + string("id"));
		double radius = cfg.lookup(prefix + string("radius"));
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
	int progress = -1;
	while (stime < duration)
	{
		int p = (((double)stime)/duration)*100.0f;
		if ( p - progress >= 15)
		{
			progress = p;
			TRI_LOG_STR("Progress: " << progress << " %");
		}

		stime += 1;
		
		// for all molecules perform their action
		for (list<Molecule*>::iterator mit = smolecules->begin(); mit != smolecules->end(); ++mit)
		{
			(*mit)->move(stime, bm->get_move());
			for (vector<RCell>::iterator cit = sreceivers->begin(); cit != sreceivers->end(); ++cit)
			{
				if ((*mit)->check_collision(&(*cit)))
				{
					// RCell->accept_molecule();
					mit = smolecules->erase(mit);
					break;
				}
			}
			//TRI_LOG_STR( *(*mit) );
		}
	}

	sfinished = true;
	TRI_LOG_STR("Finished simulation");
	TRI_LOG_STR("Finished simulation with " << smolecules->size() << " free molecules");
	// for (vector<RCell>::iterator cit = sreceivers->begin(); cit != sreceivers->end(); ++cit) {
	// 	std::vector<Molecule*>* ms = cit->molecules();
	// 	for (vector<Molecule*>::iterator mit = ms->begin(); mit != ms->end(); ++mit) {
	// 		TRI_LOG_STR( (*cit) << " " << *(*mit));
	// 	}
	// }

	TRI_LOG_STR("Simulation results:");
	// for (vector<RCell>::iterator cit = sreceivers->begin(); cit != sreceivers->end(); ++cit)
	// {
	// 	TRI_LOG_STR("\tRCell(" << cit->id()<< "): " << cit->molecules()->size() << " collided");
	// }

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

list<Molecule*>* Simulation::molecules()
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
