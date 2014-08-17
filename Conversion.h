#ifndef SIM_CONVERSION_H
#define SIM_CONVERSION_H

#include "Types.h"
#include "tri_logger/tri_logger.hpp"

class Conversion
{
private:

public:
	static constexpr double MOLECULES_UM3_TO_U_MOLES = 2.2 / 1320;

	static double molecules_um3_to_uM(uint count, Coordinate volume)
	{
		return count / volume * MOLECULES_UM3_TO_U_MOLES;
	}

};

#endif /* SIM_CONVERSION_H */
