#ifndef SIM_STATISTICS_H
#define SIM_STATISTICS_H

#include "Simulation.h"
#include "tri_logger/tri_logger.hpp"

class Statistics
{
private:
	Simulation* ssimulation;

public:
	Statistics(Simulation* s);
	void run();
};

#endif /* SIM_STATISTICS_H */
