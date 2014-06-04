#ifndef SIM_GENERATOR_H
#define SIM_GENERATOR_H

#include "Types.h"
#include "tri_logger/tri_logger.hpp"

class Generator
{
private:
	Generator();

public:

	static float data_test_Po_plus40mV(float concentration);
	static float data_test_to_plus40mV(float concentration);
	
};

#endif /* SIM_GENERATOR_H */
