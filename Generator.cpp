#include <cassert>
#include <cmath>
#include "Generator.h"


namespace
{
	const float CaC_100nM = 1e-7;
	const float CaC_1uM   = 1e-6;
	const float CaC_5uM   = 5*1e-6;
	const float CaC_10uM  = 1e-5;
	const float CaC_100uM = 1e-4;
	const float CaC_1mM   = 1e-3;

	float get_linear_proportional(float rb, float re, float vb, float ve, float r)
	{
		if (r < rb || r > re)
		{
			TRI_LOG_STR(" ERROR in get_linear_proportional()");
			return 0;
		}

		float l = re - rb;
		float h = ve - vb;
		float x = r - rb;
		return vb + h * (x / l);
	}
}

// http://aups.org.au/Proceedings/38/59-67/
// no ATP
float Generator::data_test_Po_plus40mV(float concentration)
{

	if (concentration < CaC_1uM)
	{
		return 0;
	}
	else if (concentration < CaC_5uM)
	{
		return get_linear_proportional(CaC_1uM, CaC_5uM, 0, 0.05, concentration);
	}
	else if (concentration < CaC_10uM)
	{
		return get_linear_proportional(CaC_5uM, CaC_10uM, 0.05, 0.4, concentration);
	}
	else if (concentration < CaC_100uM)
	{
		return get_linear_proportional(CaC_10uM, CaC_100uM, 0.4, 0.6, concentration);
	}
	else if (concentration < CaC_1mM)
	{
		return 0.6;
	}

	assert(false); // we can think that the cell dead ;-)
}

float Generator::data_test_to_plus40mV(float concentration)
{
	if (concentration < CaC_5uM)
	{
		return 1;
	}
	else if (concentration < CaC_10uM)
	{
		return get_linear_proportional(CaC_5uM, CaC_10uM, 1, 2, concentration);
	}
	assert(false); // check to see if this need improvement
}
