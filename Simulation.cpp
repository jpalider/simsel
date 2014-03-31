#include <iostream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <libconfig.h++>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <iterator>

#include "Simulation.h"
#include "Statistics.h"
#include "Vector.h"
#include "Molecule.h"
#include "CairoColor.h"
#include "Receptor.h"
#include "Source.h"
#include "Obstacle.h"
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

	TRI_LOG_STR("Sim: load receptor configuration");
	sreceivers = load_configuration<Receptor>("receptors");

	TRI_LOG_STR("Sim: load obstacle configuration");
	sobstacles = load_configuration<Obstacle>("obstacles");

	TRI_LOG_STR("Sim: load sources configuration");
	stransmitters = load_configuration<Source>("sources");

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
	delete sobstacles;
}

template<typename BoundaryType>
std::vector<BoundaryType>* Simulation::load_configuration(string boundary)
{
	auto v = new std::vector<BoundaryType>();
	int item_no = cfg.lookup("simulation." + boundary).getLength();

	for (int i = 0; i < item_no; i++)
	{
		stringstream ss;
		ss << i;
		string prefix = string("simulation." + boundary + ".[") + ss.str() + string("].");
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
		Id id = cfg.lookup(prefix + string("id"));
		string shape = cfg.lookup(prefix + string("shape"));
		if ( shape.find("cube") != string::npos)
		{
			TRI_LOG_STR("Sim: shape is cube [unsupported]");
		}
		else if ( shape.find("sphere") != string::npos)
		{
			TRI_LOG_STR("Sim: shape is sphere");
			Coordinate radius = cfg.lookup(prefix + string("radius"));
			radius *= ssim_scale;
			bool disabled = cfg.lookup(prefix + string("disabled"));
			if (!disabled)
			{
				v->push_back(BoundaryType(id, cp, radius));
			}
		}
		else
		{
			TRI_LOG_STR("Sim: unknown shape");
		}

		TRI_LOG_STR(boundary << " pos" << cp);
	}
	return v;

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

	vector<Boundary*> boundaries;
	for_each(sreceivers->begin(), sreceivers->end(), [&boundaries](Receptor& b){ boundaries.push_back(&b); });
	for_each(sobstacles->begin(), sobstacles->end(), [&boundaries](Obstacle& b){ boundaries.push_back(&b); });

	long repeat_counter = 0;
	long repeat_counter_again = 0;
	
	while (stime < duration)
	{
		print_progress();

		for (auto mit = smolecules->begin(); mit != smolecules->end(); )
		{
			Vector move = bm->get_move();
			(*mit)->move(move);

			bool repeat_move = false;
			for (auto cit = boundaries.begin(); cit != boundaries.end(); ++cit)
			{
				auto obstacle = *cit;
				if ((*mit)->check_collision(obstacle))
				{
					repeat_move = obstacle->collide(*mit);
					//mit = smolecules->erase(mit);
					// cit->act(*mit)
					break;
				}
			}

			if (repeat_move)
			{
				++repeat_counter;
				//TRI_LOG_STR("Repeat move");
				move = bm->get_move() / 2.f;
				(*mit)->move_back();
				(*mit)->move(move);
				for (auto cit = boundaries.begin(); cit != boundaries.end(); ++cit)
				{
					auto obstacle = *cit;
					if ((*mit)->check_collision(obstacle))
					{
						++repeat_counter_again;
						//TRI_LOG_STR("Collision during move repeat");
						continue; // means loop again on same molecule
					}
				}
			}
			++mit;
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
	TRI_LOG_STR("Finished simulation with " << repeat_counter << "repeats");
	TRI_LOG_STR("Finished simulation with " << repeat_counter_again << "repeats again");
}

void Simulation::print_progress()
{
	static int progress = -1;

	int p = (((double)stime)/duration)*100.0f;
	if ( p - progress >= 1)
	{
		progress = p;
		TRI_LOG_STR("Progress: " << progress << " %");
	}
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

vector<Receptor>* Simulation::receivers()
{
	return sreceivers;
}

vector<Source>* Simulation::transmitters()
{
	return stransmitters;
}
