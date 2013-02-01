#include <iostream>
#include <vector>

#include "Simulation.h"
#include "BrownianMotion.h"
#include "Vector.h"
#include "Molecule.h"
#include "CairoColor.h"

using namespace std;

Simulation::Simulation()
{
	sstarted = false;
	sfinished = false;
	stime = 0;
}

void Simulation::run(int nom, int noi, vector<Molecule>* molecules, vector<Cell>* recv_cells)
{
	cout << "Starting simulation" << endl;
	sstarted = true;
	BrownianMotion bm;
	Vector p(0, 0, 0);

	// add molecules to the envirnment
	for (int i = 0; i < nom; i++)
	{
		molecules->push_back(Molecule(-1, p));
	}

	// preparation of recevie cells
	for (int i = 1; i < 4; i++)
	{
		Vector cp(0,0,0);
		for (int m = 0; m < 17; m++)
		{			
			cp += bm.get_move();
		}
		recv_cells->push_back(Cell(i, cp, 14));
		cout << "cell pos: " << cp << " ";
	}
	cout << endl;


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
	cout << "Simulation finished." << endl;
	
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
