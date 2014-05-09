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
#include <pthread.h>

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

struct PthData
{
	MStoreIter              b_iter;
	MStoreIter              e_iter;
	std::vector<Boundary*> *boundaries;
	BrownianMotion         *bm;
	Obstacle               *space;
};

namespace
{

void move_molecule(Molecule* molecule, vector<Boundary*>& boundaries, BrownianMotion *bm, Obstacle *space);

bool stop_worker_threads = false;

pthread_barrier_t trigger_from_main;
pthread_barrier_t wait_from_main;


void* pth_worker(void* arg)
{
	PthData              *pthd            = (PthData*)arg;
	MStoreIter            b_iter          = pthd->b_iter;
	MStoreIter            e_iter          = pthd->e_iter;
	vector<Boundary*>&    boundaries      =*pthd->boundaries;
	BrownianMotion       *bm              = pthd->bm;
	Obstacle             *space           = pthd->space;

	while(!stop_worker_threads)
	{
		pthread_barrier_wait(&trigger_from_main);

		for (auto mit = b_iter; mit != e_iter; ++mit )
		{
			move_molecule(*mit, boundaries, bm, space);
		}
		pthread_barrier_wait(&wait_from_main);
	}
	return nullptr;
}

void move_molecule(Molecule* molecule, vector<Boundary*>& boundaries, BrownianMotion *bm, Obstacle *space)
{

	if (!molecule->is_owner(space))
	{
		return;
	}

	Vector move = bm->get_move();
	molecule->move(move);

	bool repeat_move = false;
	for (auto cit = boundaries.begin(); cit != boundaries.end(); ++cit)
	{
		auto obstacle = *cit;

		if (molecule->check_collision(obstacle))
		{
			repeat_move = obstacle->collide(molecule);
			if (!repeat_move)
			{
				obstacle->handle_collision(molecule);
			}
			break;
		}
	}

	if (repeat_move)
	{
		move /= 2;
		molecule->move_back();
		molecule->move(move);
		for (auto cit = boundaries.begin(); cit != boundaries.end(); ++cit)
		{
			auto obstacle = *cit;
			if (molecule->check_collision(obstacle))
			{
				molecule->move_back();
				break;
			}
		}
	}
}

}

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
	sthreads = 1;

	smolecules = new MStore();

	cfg.readFile("cfg/Simulation.cfg");
	string description = cfg.lookup("description");
	TRI_LOG_STR("Simulation:\n" << description);

	TRI_LOG_STR("Sim: load number of threads for parallelism");
	sthreads = cfg.lookup("simulation.threads");

	bool repetitive = cfg.lookup("simulation.repetitive");
	int seed = repetitive ? cfg.lookup("simulation.seed") : std::time(NULL);
	//srand(seed);
	TRI_LOG_STR("Repetitivity set to: " << (repetitive ? "true" : "false") << " with seed: " << seed);

	sdimensions = cfg.lookup("simulation.dimensions");
	TRI_LOG_STR("Brownian motion diemnsions set to: " << sdimensions);

	stime_step = cfg.lookup("simulation.time_step_ns");
	stau = stime_step * 1e-9;
	TRI_LOG_STR("Brownian motion time step set to: " << stau);

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
			smolecules->push_back(new Molecule(i, p, sspace));
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
	for (auto mit = smolecules->begin(); mit != smolecules->end(); ++mit)
	{
		delete (*mit);
	}

	for (auto sit = sstat.begin(); sit != sstat.end(); ++sit)
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

	const size_t range = smolecules->size() / sthreads;
	pthread_t pths[sthreads];
	PthData pth_data[sthreads];
	BrownianMotion *pth_bm[sthreads];
	auto from = std::begin(*smolecules);
	for (size_t i = 0; i < sthreads; ++i)
	{
		auto to = std::next(from, range);
		pth_bm[i] = new BrownianMotion(sdimensions, stau);
		pth_data[i] = {from, to, &boundaries, pth_bm[i], sspace };
		TRI_LOG_STR("sthread range= " << (to - from));
		from = to;
	}
	pth_data[sthreads-1].e_iter = std::end(*smolecules);

	pthread_barrier_init(&trigger_from_main, NULL, sthreads+1);
	pthread_barrier_init(&wait_from_main,    NULL, sthreads+1);

	for (size_t i = 0; i < sthreads; ++i)
	{
		pthread_create(&pths[i], NULL, pth_worker, &pth_data[i]);
	}

	pthread_barrier_wait(&trigger_from_main); // initial: let threads run

	while (stime < duration)
	{
		pthread_barrier_wait(&wait_from_main); // wait for worker threads to finish
		                                       // in order to start statistics

		print_progress();

		for (vector<Statistics*>::iterator sit = sstat.begin(); sit != sstat.end(); ++sit)
		{
			(*sit)->run(stime, smolecules, sreceivers);
		}

		stime += stime_step;

		pthread_barrier_wait(&trigger_from_main); // let worker threads continue
	}
	// worker threads run once again but we are not interesed in the results anymore

	stop_worker_threads = true;
	pthread_barrier_wait(&wait_from_main); // unlock this barrier to let worker threads exit

	int rv = 0;

	for (size_t i = 0; i < sthreads; ++i)
	{
		rv += pthread_join(pths[i], NULL);
	}

	pthread_barrier_destroy(&wait_from_main);
	pthread_barrier_destroy(&trigger_from_main);

	if (rv)
	{
		TRI_LOG_STR("Thread problem");
	}

	cout << endl;

	sfinished = true;

	for (size_t i = 0; i < sthreads; ++i)
	{
		delete pth_bm[i];
	}

	TRI_LOG_STR("Finished simulation");
	TRI_LOG_STR("Finished simulation with " << smolecules->size() << " free molecules");
	// TRI_LOG_STR("Finished simulation with " << repeat_counter << "repeats");
	// TRI_LOG_STR("Finished simulation with " << repeat_counter_again << "repeats again");
}

void Simulation::print_progress()
{
	static int progress = -1;

	int p = (((double)stime)/duration)*100.0f;
	if ( p - progress >= 1)
	{
		progress = p;
		cerr << "\rProgress: " << progress << " % ";
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

MStore* Simulation::molecules()
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
