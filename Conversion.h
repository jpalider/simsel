#ifndef SIM_CONVERSION_H
#define SIM_CONVERSION_H
#include <cmath>
#include "Types.h"
#include "tri_logger/tri_logger.hpp"

class Conversion
{
private:

public:
	static constexpr double MOLECULES_UM3_TO_U_MOLES = 2.2 / 1320;
	static constexpr double M3_TO_UM3 = pow(10, 3*6);
	static constexpr double uM = pow(10, -6);

	static double molecules_um3_to_uM(uint count, Coordinate volume)
	{
		return count / volume /*** / M3_TO_UM3 ***/ * MOLECULES_UM3_TO_U_MOLES;
	}

	static double pA_to_molecules(double pA)
	{
		return pA * 500*1000 / 160;
	}

	static double molecules_to_pA(uint count)
	{
		return count * 160 / 500*1000 ;
	}

	static Time ms_to_ns(Time count)
	{
		return count*1000*1000 ;
	}

	static Time ns_to_ms(Time count)
	{
		return count/1000/1000 ;
	}

};

#endif /* SIM_CONVERSION_H */
