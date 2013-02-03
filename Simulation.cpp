#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <libconfig.h++>

#include "Simulation.h"
#include "BrownianMotion.h"
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
}

void Simulation::run(int nom, int noi, vector<Molecule>* molecules, vector<Cell>* recv_cells)
{
	TRI_LOG_STR("Starting simulation");

	libconfig::Config cfg;
	cfg.readFile("cfg/Simulation.cfg");
	string description = cfg.lookup("description");
	sstarted = true;
	BrownianMotion bm;
	Vector p(0, 0, 0);

	// add molecules to the envirnment
	for (int i = 0; i < nom; i++)
	{
		molecules->push_back(Molecule(i, p));
	}

	// preparation of recevie cells
	int cells_no = cfg.lookup("simulation.cells").getLength();
	TRI_LOG(cells_no);

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
		recv_cells->push_back(Cell(id, cp, 14));
		TRI_LOG_STR("cell pos" << cp);
	}


	// perform simulation iterations
	for (int i = 0; i < noi; i++)
	{
		stime += 1;
		// for all molecules perform their action
		for (vector<Molecule>::iterator mit = molecules->begin(); mit != molecules->end(); ++mit) {
			mit->move(stime, bm.get_move(300));
			for (vector<Cell>::iterator cit = recv_cells->begin(); cit != recv_cells->end(); ++cit) {
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
