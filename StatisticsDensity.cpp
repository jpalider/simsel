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
#include "Types.h"
#include "Conversion.h"
#include "tri_logger/tri_logger.hpp"

using namespace std;

StatisticsDensity::StatisticsDensity(Simulation *s, long interval) : Statistics(s, interval)
{
}

void StatisticsDensity::run(long time, const MStore* const molecules, const std::vector<Receptor>* const rcells)
{
	static bool first_run = true;

	if (time % sinterval == 0)
	{
		if (first_run)
		{
			for (vector<Receptor>::const_iterator cit = rcells->begin(); cit != rcells->end(); ++cit)
			{
				ofstream stat_stream;
				stringstream ss;
				ss << "_rcell_" << cit->id();

				stat_stream.open(filename(ss.str()).c_str(), ios::app);
				stat_stream << "# " << *cit << endl;
				stat_stream << "# time molecule_count_per_volume molecule_count umoles" << endl;
				stat_stream.close();
			}
			first_run = false;
		}

		for (vector<Receptor>::const_iterator cit = rcells->begin(); cit != rcells->end(); ++cit)
		{
			const Coordinate units_um = 1e-3;
			const Coordinate diameter = cit->radius();
			const Coordinate vol_um3  = sphere_volume(diameter / sscale * units_um); // back to nm and then to um
			int m_count = 0;
			for (auto it = molecules->begin(); it != molecules->end(); ++it)
			{
				auto m = *it;
				if (cit->has_inside(&m))
				{
					m_count++;
				}
			}

			ofstream stat_stream;
			stringstream ss;
			ss << "_rcell_" << cit->id();
			stat_stream.open(filename(ss.str()).c_str(), ios::app);
			stat_stream << time * sscale
				    << " " << ( m_count / vol_um3                                 )
				    << " " << ( m_count                                           )
				    << " " << ( Conversion::molecules_um3_to_uM(m_count, vol_um3) )
				    << endl;
			stat_stream.close();
		}
	}
}
