#include <list>
#include <map>
#include <cmath>

#include "Statistics.h"
#include "Vector.h"
#include "Molecule.h"
#include "Math.h"
#include "tri_logger/tri_logger.hpp"

//#include "gnuplot-iostream/gnuplot-iostream.h"

using namespace std;

Statistics::Statistics(double sim_scale)
{
	ssim_scale = sim_scale;
}

void Statistics::run(long time, const std::list<Molecule*>* const molecules, const std::vector<RCell>* const rcells)
{
	long time1 = 90;

	Vector init(0.,0.,0.);
	const int MAX_DISTANCE = 300000;

	for (int timex = 0; timex < 50000; timex++)
	{
		
		int distance[MAX_DISTANCE] = {};

		for (list<Molecule*>::const_iterator it = molecules->begin(); it != molecules->end(); ++it)
		{
			const map<long, Vector>* h = (*it)->histogram();
			map<long, Vector>::const_iterator pit = h->lower_bound(timex);

			double d = squared_distance_between_points(&init, &(pit->second));
			d = sqrt(d);
			d *= ssim_scale;
//		cout << d << " " << (pit->second).x << endl;
			if ( (int)d < MAX_DISTANCE)
			{
				distance[(int)d]++;
			}
		}

		cout << endl <<  "+---------------------------------------------------->" << endl;
		for (int i = 0; i < MAX_DISTANCE; i++)
		{
			cout << "|";
			int f = distance[i] / 10000;
			for (int n = 0; n < f; n++)
			{
				cout << "*";
			}
			cout << endl;
		}
		cout << "v" << endl;
		char c;
		cin.get();

	}
//	return ;
	
}
