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
bool move_molecule_in_space(Molecule *molecule, BrownianMotion *bm, Obstacle *space, Vector *move);
void join_worker_threads(pthread_t* pts, size_t ptsn);
void setup_worker_threads(PthData *ptd, size_t ptsn, MStore *ms, BrownianMotion **ptbm, vector<Boundary*> *boundaries, Obstacle *space);
void create_worker_threads(pthread_t* pts, size_t ptsn, PthData *ptd);

bool stop_worker_threads = false;
bool balance_worker_threads = false;

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
		if (balance_worker_threads)
		{
			return nullptr;
		}

		for (auto mit = b_iter; mit != e_iter; ++mit )
		{
			move_molecule(&*mit, boundaries, bm, space);
		}
		pthread_barrier_wait(&wait_from_main);
	}
	return nullptr;
}

// Return true on successful move within the boundary. False return value indicates
// move outside the border which mean - in such case original position is restored,
// as if nothing happened.
bool move_molecule_in_space(Molecule *molecule, BrownianMotion *bm, Obstacle *space, Vector *move)
{
	*move = bm->get_move();
	molecule->move(*move);
	// after move check for space boundaries (twice)
	if (space && !space->has_inside(molecule))
	{
		molecule->move_back();
		*move = bm->get_move();
		molecule->move(*move);
		if (!space->has_inside(molecule))
		{
			molecule->move_back();
			return false;
		}
	}

	return true;
}

void move_molecule(Molecule* molecule, vector<Boundary*>& boundaries, BrownianMotion *bm, Obstacle *space)
{

	if (!molecule->is_owner(space))
	{
		return;
	}
	Vector move;
	bool moved = move_molecule_in_space(molecule, bm, space, &move);
	if (!moved)
	{
		// no need to do any more collision checks
		return;
	}

	Boundary *bouncer = nullptr;
	// First try - look for the first one that bounces the molecule.
	// For complex structures this will not work, but sparse structures shall be fine
	for (auto cit = boundaries.begin(); cit != boundaries.end(); ++cit)
	{
		auto obstacle = *cit;

		if (obstacle->id() == 1)
			continue;

		// if molecule crossed the boundary
		if (obstacle->check_collision(molecule))
		{
			if (obstacle->collide(molecule))
			{
				bouncer = obstacle;
				// First obstacle collided indicates
				break;
			}
			else
			{
				// if can cross the boundary freely
				// handle it here, and for simplicity skip other obstacles
				obstacle->handle_collision(molecule);
				return;
			}
		}
	}

	if (bouncer)
	{
		molecule->move_back();
		move /= 2;
		molecule->move(move);
		if (bouncer->check_collision(molecule))
		{
			// if half of the first move does not help leave it as is
			// note: space with highier obstacle density may have problems
			molecule->move_back();
		}
	}

}

void join_worker_threads(pthread_t* pts, size_t ptsn)
{
	int irv = 0;
	for (size_t i = 0; i < ptsn; ++i)
	{
		irv += pthread_join(pts[i], NULL);
	}
	if (irv)
	{
		TRI_LOG_STR("Thread problem : joining");
	}
}

void setup_worker_threads(PthData *ptd, size_t ptsn, MStore *ms, BrownianMotion **ptbm, vector<Boundary*> *boundaries, Obstacle *space)
{
	size_t range = ms->size() / ptsn;
	auto from = std::begin(*ms);
	for (size_t i = 0; i < ptsn; ++i)
	{
		auto to = std::next(from, range);
		ptd[i] = { from, to, boundaries, ptbm[i], space };
		from = to;
	}
	ptd[ptsn-1].e_iter = std::end(*ms);
}

void create_worker_threads(pthread_t* pts, size_t ptsn, PthData *ptd)
{
	for (size_t i = 0; i < ptsn; ++i)
	{
		pthread_create(&pts[i], NULL, pth_worker, &ptd[i]);
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
	TRI_LOG_STR("Simulation: " << description);

	sthreads = cfg.lookup("simulation.threads");
	TRI_LOG_STR("Worker threads: " << sthreads);

	bool repetitive = cfg.lookup("simulation.repetitive");
	int seed = repetitive ? cfg.lookup("simulation.seed") : std::time(NULL);

	TRI_LOG_STR("Repetitivity (not validated): " << (repetitive ? "true" : "false") << " with seed: " << seed);

	sdimensions = cfg.lookup("simulation.dimensions");
	TRI_LOG_STR("Brownian motion dimensions: " << sdimensions);

	stime_step = cfg.lookup("simulation.time_step_ns");
	stau = stime_step * 1e-9;
	TRI_LOG_STR("Brownian motion time step: " << stau << " seconds");

	// currently cannot mix generation types
	string type = cfg.lookup("simulation.molecules.type");
	TRI_LOG_STR("Molecule generation type: " << type);

	TRI_LOG_STR("Load receptor configuration...");
	sreceivers = load_configuration<Receptor>("receptors");

	TRI_LOG_STR("Loading obstacle configuration...");
	sobstacles = load_configuration<Obstacle>("obstacles");

	TRI_LOG_STR("Loading sources configuration...");
	stransmitters = load_configuration<Source>("sources");

	TRI_LOG_STR("Loading enclosure configuration...");
	try {
		sspace = &load_configuration<Obstacle>("volume")->at(0);
	} catch(...)
	{
		TRI_LOG_STR("No enclosure.");
		sspace = nullptr;
	}

	if ( type.compare("interval") == 0 )
	{
		generation.interval.interval = cfg.lookup("simulation.molecules.interval");

		generation.interval.series = cfg.lookup("simulation.molecules.series");

		generation.interval.number = cfg.lookup("simulation.molecules.number");
		Vector p(0, 0, 0);
		for (int i = 0; i < generation.interval.number; i++)
		{
			smolecules->push_back(Molecule(i, p, sspace));
		}

		generation.interval.transmitter = cfg.lookup("simulation.molecules.transmitter");
		
		TRI_LOG_STR("Number of molecules added to the environment: " << generation.interval.number);
	}

	duration = cfg.lookup("simulation.duration");
	TRI_LOG_STR("Duration: " << duration << " seconds");
}

Simulation::~Simulation()
{
	for (auto mit = smolecules->begin(); mit != smolecules->end(); ++mit)
	{
		delete (&*mit);
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

		if (static_cast<bool>(cfg.lookup(prefix + string("disabled"))))
		{
			continue;
		}

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
		BoundaryType* new_boundary;
		if ( shape.find("cube") != string::npos)
		{
			string param_x = prefix + string("size.x");
			string param_y = prefix + string("size.y");
			string param_z = prefix + string("size.z");
			double x = cfg.lookup(param_x);
			double y = cfg.lookup(param_y);
			double z = cfg.lookup(param_z);
			x *= ssim_scale;
			y *= ssim_scale;
			z *= ssim_scale;
			new_boundary = new BoundaryType(id, cp, x, y, z);
		}
		else if ( shape.find("sphere") != string::npos)
		{
			Coordinate radius = cfg.lookup(prefix + string("radius"));
			radius *= ssim_scale;
			new_boundary = new BoundaryType(id, cp, radius);
		}
		else
		{
			TRI_LOG_STR("Sim: unknown shape");
		}

		v->push_back(*new_boundary);

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

	pthread_t pths[sthreads];
	PthData pth_data[sthreads];
	BrownianMotion *pth_bm[sthreads];
	for (size_t i = 0; i < sthreads; ++i)
	{
		pth_bm[i] = new BrownianMotion(sdimensions, stau);
	}

	setup_worker_threads(pth_data, sthreads, smolecules, pth_bm, &boundaries, sspace);

	pthread_barrier_init(&trigger_from_main, NULL, sthreads+1);
	pthread_barrier_init(&wait_from_main,    NULL, sthreads+1);

	create_worker_threads(pths, sthreads, pth_data);

	pthread_barrier_wait(&trigger_from_main); // initial: let threads run

	while (stime < duration)
	{
		// the processing in other threads takes part between barriers:
		// after 'trigger_from_main' at the end of this loop
		// and before 'wait_from_main' at the beginning

		pthread_barrier_wait(&wait_from_main); // wait for worker threads to finish
		                                       // in order to start statistics

		print_progress();

		for (vector<Statistics*>::iterator sit = sstat.begin(); sit != sstat.end(); ++sit)
		{
			(*sit)->run(stime, smolecules, sreceivers);
		}

		stime += stime_step;

		for (vector<Source>::iterator tit = stransmitters->begin(); tit != stransmitters->end(); ++tit)
		{
			if (tit->release(stime, smolecules, sspace))
			{
				balance_worker_threads = true;
			}
		}

		pthread_barrier_wait(&trigger_from_main); // let worker threads continue

		if (balance_worker_threads)
		{
			join_worker_threads(pths, sthreads);
			// it is safe now to clear the flag as no other thread reads it
			balance_worker_threads = false;
			setup_worker_threads(pth_data, sthreads, smolecules, pth_bm, &boundaries, sspace);
			create_worker_threads(pths, sthreads, pth_data);

			pthread_barrier_wait(&trigger_from_main); // start worker threads
			TRI_LOG_STR("Balancing: done");
		}

	}
	// worker threads run once again but we are not interesed in the results anymore

	stop_worker_threads = true;
	pthread_barrier_wait(&wait_from_main); // unlock this barrier to let worker threads exit

	join_worker_threads(pths, sthreads);

	pthread_barrier_destroy(&wait_from_main);
	pthread_barrier_destroy(&trigger_from_main);


	cout << endl;

	sfinished = true;

	for (size_t i = 0; i < sthreads; ++i)
	{
		delete pth_bm[i];
	}

	TRI_LOG_STR("Finished simulation");
	TRI_LOG_STR("Finished simulation with " << smolecules->size() << " free molecules");
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
