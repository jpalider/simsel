#ifndef SIM_STATISTICS_H
#define SIM_STATISTICS_H

#include <list>
#include <vector>

#include "Molecule.h"
#include "RCell.h"

class Simulation;

class Statistics
{
protected:
	double sscale;
public:
	Statistics(Simulation* s);
	virtual void run(long time, const std::list<Molecule*>* const molecules, const std::vector<RCell>* const rcells) = 0;
protected:
	std::string current_time_as_string();
};

#endif /* SIM_STATISTICS_H */
