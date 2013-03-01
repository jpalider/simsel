#ifndef SIM_STATISTICS_DISTRIBUTION_H
#define SIM_STATISTICS_DISTRIBUTION_H

#include "Simulation.h"

class StatisticsDistribution : public Statistics
{
private:

public:
	StatisticsDistribution(Simulation* s);
	void run(long time, const std::list<Molecule*>* const molecules, const std::vector<RCell>* const rcells);
};

#endif /* SIM_STATISTICS_DISTRIBUTION_H */
