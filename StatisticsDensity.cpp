#include <list>
#include <map>
#include <cmath>
#include <fstream>
#include <sstream>
#include <strstream>
#include <string>

#include "StatisticsDensity.h"
#include "Vector.h"
#include "Molecule.h"
#include "Math.h"
#include "tri_logger/tri_logger.hpp"

using namespace std;

/**
 * Density at 3um distance
 */
StatisticsDensity::StatisticsDensity(Simulation *s, long interval) : Statistics(s, interval)
{
}

void StatisticsDensity::run(long time, const std::list<Molecule*>* const molecules, const std::vector<Receptor>* const rcells)
{

	static const string time_prefix = current_time_as_string();

	static const double vol = sphere_volume(0.4); // um, just in this test statistics

	if (time % sinterval == 0)
	{
		const Vector* v = NULL;
		for (vector<Receptor>::const_iterator cit = rcells->begin(); cit != rcells->end(); ++cit)
		{
			int m_count = 0;
			double diameter = cit->radius();
			diameter = diameter*diameter;

			for (list<Molecule*>::const_iterator it = molecules->begin(); it != molecules->end(); ++it)
			{
				v = (*it)->position();
				double d = squared_distance_between_points(cit->position(), v);
				//double d = squared_distance_between_points(&init, v);
				if ( d < diameter )
				{
					m_count++;
				}
			}

			ofstream stat_stream;
			stringstream ss;
			ss << "_rcell_" << cit->id();
			stat_stream.open((string("results/") + time_prefix + ss.str() + string(".dat")).c_str(), ios::app);
			stat_stream << time * sscale << " " << ( m_count / vol ) << " " << m_count << endl;
			stat_stream.close();

		}
	}
}
