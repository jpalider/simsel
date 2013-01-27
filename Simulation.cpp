#include <iostream>
#include <vector>

#include "Simulation.h"
#include "BrownianMotion.h"
#include "Vector.h"
#include "Molecule.h"
#include "CairoColor.h"

using namespace std;

void Simulation::simulate(int nom, int noi, vector<Molecule>* molecules)
{
	cout << "Starting simulation" << endl;
	BrownianMotion bm;
	Vector p(0, 0, 0);
	for (int i = 0; i < nom; i++)
	{
		molecules->push_back(Molecule(p, 1, CairoColor(0.69, 0.19, 0)));
	}
	

	long sim_time = 0;
	for (int i = 0; i < noi; i++)
	{
		sim_time += 5;
		for (vector<Molecule>::iterator it = molecules->begin(); it != molecules->end(); ++it) {
			it->move(sim_time, bm.get_move());				
		}
	}
	cout << "Simulation finished." << endl;
	
}
