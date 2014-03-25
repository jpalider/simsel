#ifndef SIM_STATISTICS_DENSITY_H
#define SIM_STATISTICS_DENSITY_H

#include "Simulation.h"

class StatisticsDensity : public Statistics
{
private:

public:
	StatisticsDensity(Simulation* s, long interval);
	void run(long time, const std::list<Molecule*>* const molecules, const std::vector<Receptor>* const rcells);
};

#endif /* SIM_STATISTICS_DENSITY_H */
