#include <iostream>
#include <vector>

#include "Simulation.h"
#include "BrownianMotion.h"
#include "Vector.h"
#include "Molecule.h"
#include "CairoColor.h"

using namespace std;

void Simulation::simulate(int nom, int noi, vector<Molecule>* molecules, vector<Cell>* recv_cells)
{
	cout << "Starting simulation" << endl;
	BrownianMotion bm;
	Vector p(0, 0, 0);
	for (int i = 0; i < nom; i++)
	{
		molecules->push_back(Molecule(-1, p, CairoColor(0.79f, 0.39f, 0.19f)));
	}

	for (int i = 1; i < 4; i++)
	{
		Vector cp(0,0,0);
		for (int m = 0; m < 17; m++)
		{			
			cp += bm.get_move();
		}
		recv_cells->push_back(Cell(i, cp, 14, CairoColor(0.19, 0.69, 0)));
		cout << "cell pos: " << cp << " ";
	}
	cout << endl;

	long sim_time = 0;
	for (int i = 0; i < noi; i++)
	{
		sim_time += 5;
		for (vector<Molecule>::iterator mit = molecules->begin(); mit != molecules->end(); ++mit) {
			mit->move(sim_time, bm.get_move(300));
			for (vector<Cell>::iterator cit = recv_cells->begin(); cit != recv_cells->end(); ++cit) {
				mit->check_collision(&(*cit));
			}
		}
	}
	cout << "Simulation finished." << endl;
	
}
