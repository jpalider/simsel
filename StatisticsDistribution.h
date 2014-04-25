#ifndef SIM_STATISTICS_DISTRIBUTION_H
#define SIM_STATISTICS_DISTRIBUTION_H

#include "Types.h"
#include "Simulation.h"

class StatisticsDistribution : public Statistics
{
private:

public:
	StatisticsDistribution(Simulation* s, long interval);
	void run(long time, const MStore* const molecules, const std::vector<Receptor>* const rcells);
};

#endif /* SIM_STATISTICS_DISTRIBUTION_H */
