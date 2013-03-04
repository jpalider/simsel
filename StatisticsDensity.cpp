#include <list>
#include <map>
#include <cmath>
#include <fstream>
#include <sstream>
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

void StatisticsDensity::run(long time, const std::list<Molecule*>* const molecules, const std::vector<RCell>* const rcells)
{

	static const string time_prefix = current_time_as_string();

	static const Vector init(0.,0.,0.);

	static const int MEASURED_DISTANCE = 3*1000; // um

	static const int outer_radius = 3100;
	
	static const int inner_radius = 2900;

	static const double outer_radius_um = outer_radius / 1000.;

	static const double inner_radius_um = inner_radius / 1000.;

	static const double vol = 4. / 3. * M_PI * ( pow(outer_radius_um, 3) - pow(inner_radius_um, 3) );
	
	if (time % sinterval == 0)
	{
		const Vector* v = NULL;
		int m_count = 0;

		for (list<Molecule*>::const_iterator it = molecules->begin(); it != molecules->end(); ++it)
		{
			v = (*it)->position();
			double d = squared_distance_between_points(&init, v);
			d = sqrt(d);
			d /= sscale; // now in [nm]
			if (d > inner_radius && d < outer_radius)
			{
				m_count++;
			}
		}

		ofstream stat_stream;
		stat_stream.open((string("results/") + time_prefix + string("_dens_") + string(".txt")).c_str(), ios::app);

		stat_stream << time << " " << ( m_count / vol ) << endl;
		stat_stream.close();

	}
}
