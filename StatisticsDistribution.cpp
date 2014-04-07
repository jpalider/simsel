#include <list>
#include <map>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

#include "StatisticsDistribution.h"
#include "Vector.h"
#include "Molecule.h"
#include "Math.h"
#include "tri_logger/tri_logger.hpp"

using namespace std;

StatisticsDistribution::StatisticsDistribution(Simulation *s, long interval) : Statistics(s, interval)
{
}

void StatisticsDistribution::run(long time, const std::list<Molecule*>* const molecules, const std::vector<Receptor>* const rcells)
{

	static const string time_prefix = current_time_as_string();

	static const Vector init(0.,0.,0.);

	static const int MAX_DISTANCE = 5*1000; // um

	if (time % sinterval == 0)
	{

		int molecules_at_distance[MAX_DISTANCE] = {};
		const Vector* v = nullptr;

		for (list<Molecule*>::const_iterator it = molecules->begin(); it != molecules->end(); ++it)
		{
			v = (*it)->position();
			double d = squared_distance_between_points(&init, v);
			d = sqrt(d);
			d /= sscale; // now in [nm]
			// d /= 1000; // now in [um]
			int floored_value = (int)d;
			if (floored_value < MAX_DISTANCE)
			{
				molecules_at_distance[floored_value]++;
			}
		}

		stringstream ss;
		ss << time;
		ofstream stat_stream;
		stat_stream.open((string("results/") + time_prefix + string("_dist_") + ss.str() + string(".txt")).c_str());

		for (int i = 0; i < MAX_DISTANCE; i++)
		{
			int f = molecules_at_distance[i]; // real value
			stat_stream << i << " " <<  f << "\n";
		}
		stat_stream << endl;
		stat_stream.close();

	}
}
