#ifndef SIM_STATISTICS_H
#define SIM_STATISTICS_H

#include <list>
#include <vector>

#include "Molecule.h"
#include "RCell.h"
#include "tri_logger/tri_logger.hpp"

class Statistics
{
private:
	double ssim_scale;
public:
	Statistics(double sim_scale);
	void run(long time, const std::list<Molecule*>* const molecules, const std::vector<RCell>* const rcells);
};

#endif /* SIM_STATISTICS_H */
