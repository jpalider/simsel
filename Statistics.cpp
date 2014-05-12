#include <list>
#include <map>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

#include "Statistics.h"
#include "Simulation.h"
#include "Vector.h"
#include "Molecule.h"
#include "Math.h"
#include "tri_logger/tri_logger.hpp"

using namespace std;

Statistics::Statistics(Simulation* s, long interval)
{
	sscale = s->scale();
	sinterval = interval;
}

Statistics::~Statistics()
{
}

string Statistics::filename(string custom)
{
	static const string time_prefix = current_time_as_string();

	return string("results/") + time_prefix + custom + string(".dat");
}

string Statistics::current_time_as_string()
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [80];

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime (buffer,80,"%Y-%m-%d-%H-%M",timeinfo);

	return string(buffer);
}
