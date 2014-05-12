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

StatisticsDensity::StatisticsDensity(Simulation *s, long interval) : Statistics(s, interval)
{
}

void StatisticsDensity::run(long time, const MStore* const molecules, const std::vector<Receptor>* const rcells)
{
	if (time % sinterval == 0)
	{
		const Vector* v = nullptr;
		const double units_um = 0.001;
		for (vector<Receptor>::const_iterator cit = rcells->begin(); cit != rcells->end(); ++cit)
		{
			const double diameter   = cit->radius();
			const double sqdiameter = diameter*diameter;
			const double vol        = sphere_volume(diameter / sscale * units_um); // back to nm and then to um
			int m_count = 0;

			for (auto it = molecules->begin(); it != molecules->end(); ++it)
			{
				v = (*it)->position();
				double sqd = squared_distance_between_points(cit->position(), v);
				if ( sqd < sqdiameter )
				{
					m_count++;
				}
			}

			ofstream stat_stream;
			stringstream ss;
			ss << "_rcell_" << cit->id();
			stat_stream.open(filename(ss.str()).c_str(), ios::app);
			stat_stream << time * sscale << " " << ( m_count / vol ) << " " << m_count << endl;
			stat_stream.close();

		}
	}
}
