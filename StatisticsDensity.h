#ifndef SIM_STATISTICS_DENSITY_H
#define SIM_STATISTICS_DENSITY_H

#include "Types.h"
#include "Simulation.h"

class StatisticsDensity : public Statistics
{
private:

public:
	StatisticsDensity(Simulation* s, long interval);
	void run(long time, const MStore* const molecules, const std::vector<Receptor>* const rcells);
};

#endif /* SIM_STATISTICS_DENSITY_H */
