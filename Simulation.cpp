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
#include "Statistics.h"
#include "Vector.h"
#include "Molecule.h"
#include "CairoColor.h"
#include "RCell.h"
#include "TCell.h"
#include "Math.h"
#include "tri_logger/tri_logger.hpp"

using namespace std;

struct Interval
{
	long interval;
	int series;
	int number;
	int transmitter;
};


union Generation
{
	struct Interval interval;
} generation;

Simulation::Simulation()
{
	ssim_scale = 1e-9;

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

	stime_step = cfg.lookup("simulation.time_step_ns");
	double tau = stime_step * 1e-9;
	TRI_LOG_STR("Brownian motion time step set to: " << tau);

	bm = new BrownianMotion(dimensions, tau);

	// currently cannot mix generation types
	string type = cfg.lookup("simulation.molecules.type");
	TRI_LOG_STR("Getting molecule generation type: " << type);

	if ( type.compare("interval") == 0 )
	{
		generation.interval.interval = cfg.lookup("simulation.molecules.interval");

		generation.interval.series = cfg.lookup("simulation.molecules.series");

		generation.interval.number = cfg.lookup("simulation.molecules.number");
		Vector p(0, 0, 0);
		for (int i = 0; i < generation.interval.number; i++)
		{
			smolecules->push_back(new Molecule(i, p));
		}

		generation.interval.transmitter = cfg.lookup("simulation.molecules.transmitter");
		
		TRI_LOG_STR("Number of molecules added to the environment: " << generation.interval.number);
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
		double x = cfg.lookup(param_x);
		double y = cfg.lookup(param_y);
		double z = cfg.lookup(param_z);
		x *= ssim_scale;
		y *= ssim_scale;
		z *= ssim_scale;
		Vector cp(x, y, z);
		int id = cfg.lookup(prefix + string("id"));
		double radius = cfg.lookup(prefix + string("radius"));
		radius *= ssim_scale;
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
		x *= ssim_scale;
		y *= ssim_scale;
		z *= ssim_scale;
		Vector cp(x, y, z);
		int id = cfg.lookup(prefix + string("id"));
		double radius = cfg.lookup(prefix + string("radius"));
		radius *= ssim_scale;
		stransmitters->push_back(TCell(id, cp, radius));
		TRI_LOG_STR("tcell pos" << cp);
	}

	TRI_LOG_STR("Sim: load duration");
	duration = cfg.lookup("simulation.duration");
}

Simulation::~Simulation()
{
	for (list<Molecule*>::iterator mit = smolecules->begin(); mit != smolecules->end(); ++mit)
	{
		delete (*mit);
	}

	for (vector<Statistics*>::iterator sit = sstat.begin(); sit != sstat.end(); ++sit)
	{
		delete (*sit);
	}

	delete bm;
	delete stransmitters;
	delete sreceivers;
	delete smolecules;
}

double Simulation::scale()
{
	return ssim_scale;
}

void Simulation::add(Statistics* statistics)
{
	sstat.push_back(statistics);
}

void Simulation::run()
{
	sstarted = true;

	TRI_LOG_STR("Starting simulation");
	int progress = -1;
	bool added = false;

	
	
	while (stime < duration)
	{
		int p = (((double)stime)/duration)*100.0f;
		if ( p - progress >= 1)
		{
			progress = p;
			TRI_LOG_STR("Progress: " << progress << " %");
		}

		if (!added)
		{
			if (stime > 10000)
			{
				added = true;
				int mol = smolecules->size();
				Vector p(0, 0, 0);
				for (int i = 0; i < mol; i++)
				{
					smolecules->push_back(new Molecule(i+mol, p));
				}
			}
		}

		// for all molecules perform their action
		for (list<Molecule*>::iterator mit = smolecules->begin(); mit != smolecules->end(); ++mit)
		{
			(*mit)->move(stime, bm->get_move());
			// for (vector<RCell>::iterator cit = sreceivers->begin(); cit != sreceivers->end(); ++cit)
			// {
			// 	// should check sector it belongs to
			// 	// if ((*mit)->precheck_collision(&(*cit)))
				
			// 	if ((*mit)->check_collision(&(*cit)))
			// 	{
			// 		// RCell->accept_molecule();
			// 		mit = smolecules->erase(mit);
			// 		break;
			// 	}
			// }
		}

		for (vector<Statistics*>::iterator sit = sstat.begin(); sit != sstat.end(); ++sit)
		{
			(*sit)->run(stime, smolecules, sreceivers);

		}

		stime += stime_step;
	}

	sfinished = true;
	TRI_LOG_STR("Finished simulation");
	TRI_LOG_STR("Finished simulation with " << smolecules->size() << " free molecules");
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

long Simulation::interval()
{
	return stime_step;
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
