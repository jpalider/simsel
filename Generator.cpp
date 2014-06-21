#include <cassert>
#include <cmath>
#include "Generator.h"


namespace
{
	const float CaC_log_100nM = -7;
	const float CaC_log_500nM = -6.5;
	const float CaC_log_1uM   = -6;
	const float CaC_log_5uM   = -5.5;
	const float CaC_log_10uM  = -5;
	const float CaC_log_100uM = -4;
	const float CaC_log_1mM   = -3;

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

	float get_logarithmic_linear_proportional(float rb, float re, float vb, float ve, float r)
	{
		float logr = std::log10(r);
		float ret = get_linear_proportional(rb, re, vb, ve, logr);
		return ret;
	}

	float get_logarithmic_logarithmic_proportional(float rb, float re, float vb, float ve, float r)
	{
		float ret = get_logarithmic_linear_proportional(rb, re, vb, ve, r);
		float powret = std::pow(10, ret);
		return powret;
	}


}

// http://aups.org.au/Proceedings/38/59-67/
// no ATP
float Generator::data_test_Po_plus40mV(float concentration)
{

	if (concentration < CaC_log_1uM)
	{
		return 0;
	}
	else if (concentration < CaC_log_5uM)
	{
		return get_logarithmic_linear_proportional(CaC_log_1uM, CaC_log_5uM, 0, 0.05, concentration);
	}
	else if (concentration < CaC_log_10uM)
	{
		return get_logarithmic_linear_proportional(CaC_log_5uM, CaC_log_10uM, 0.05, 0.4, concentration);
	}
	else if (concentration < CaC_log_100uM)
	{
		return get_logarithmic_linear_proportional(CaC_log_10uM, CaC_log_100uM, 0.4, 0.6, concentration);
	}
	else if (concentration < CaC_log_1mM)
	{
		return 0.6;
	}

	assert(false); // we can think that the cell dead ;-)
}

float Generator::data_test_to_plus40mV(float concentration)
{
	if (concentration < CaC_log_5uM)
	{
		return 1;
	}
	else if (concentration < CaC_log_10uM)
	{
		return get_logarithmic_linear_proportional(CaC_log_5uM, CaC_log_10uM, 1, 2, concentration);
	}
	assert(false); // check to see if this need improvement
}

float Generator::data_test_tc_plus40mV(float concentration)
{
	// no data available
	// assert(concentration >= CaC_log_100nM && concentration <= CaC_log_10uM);
	return get_logarithmic_logarithmic_proportional(-7, -5, -1, 3, concentration);
}
