#ifndef SIM_GENERATOR_H
#define SIM_GENERATOR_H

#include "Types.h"
#include "tri_logger/tri_logger.hpp"

class Generator
{
private:
	Generator();

public:
	// x_ - no ATP
	static float x_Po_plus40mV(float concentration);
	static float x_to_plus40mV(float concentration);
	// static float x_tc_plus40mV(float concentration);
	static float x_or_plus40mV(float concentration);

	// o_ - with ATP
	static float o_Po_plus40mV(float concentration);
	static float o_to_plus40mV(float concentration);
	// static float o_tc_plus40mV(float concentration);
	static float o_or_plus40mV(float concentration);
};

#endif /* SIM_GENERATOR_H */
