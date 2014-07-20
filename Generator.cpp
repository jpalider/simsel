#include <cassert>
#include <cmath>
#include "Generator.h"


namespace
{
	const float CaC_log_100nM = -7;
	const float CaC_log_300nM = -6.5;
	const float CaC_log_1uM   = -6;
	const float CaC_log_3uM   = -5.5;
	const float CaC_log_10uM  = -5;
	const float CaC_log_100uM = -4;
	const float CaC_log_1mM   = -3;
	const float CaC_log_10mM  = -2;
	const float CaC_log_100mM = -1;

	const float CaC_100nM = pow(10, -7);
	const float CaC_300nM = pow(10, -6.5);
	const float CaC_1uM   = pow(10, -6);
	const float CaC_3uM   = pow(10, -5.5);
	const float CaC_10uM  = pow(10, -5);
	const float CaC_100uM = pow(10, -4);
	const float CaC_1mM   = pow(10, -3);
	const float CaC_10mM  = pow(10, -2);
	const float CaC_100mM = pow(10, -1);

	float get_linear_proportional(float rb, float re, float vb, float ve, float r)
	{
		if (r < rb || r > re)
		{
			TRI_LOG_STR(" ERROR in get_linear_proportional(): " << rb << " " << r << " " << re);
		}
		assert(r >= rb && r <= re);

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
		float ret = get_logarithmic_linear_proportional(rb, re, std::log10(vb), std::log10(ve), r);
		float powret = std::pow(10, ret);
		return powret;
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
	else if (concentration < CaC_3uM)
	{
		return get_logarithmic_linear_proportional(CaC_log_1uM, CaC_log_3uM, 0, 0.05, concentration);
	}
	else if (concentration < CaC_10uM)
	{
		return get_logarithmic_linear_proportional(CaC_log_3uM, CaC_log_10uM, 0.05, 0.4, concentration);
	}
	else if (concentration < CaC_100uM)
	{
		return get_logarithmic_linear_proportional(CaC_log_10uM, CaC_log_100uM, 0.4, 0.6, concentration);
	}
	else if (concentration < CaC_1mM)
	{
		return 0.6;
	}
	else  if (concentration < CaC_100mM)
	{
		return get_logarithmic_linear_proportional(CaC_log_1mM, CaC_log_100mM, 0.6, 0.05, concentration);
	}

	assert(false); // we do not expect other ranges
}

float Generator::data_test_to_plus40mV(float concentration)
{
	if (concentration < CaC_100nM)
	{
		assert(concentration >= CaC_100nM);
	}
	else if (concentration < CaC_3uM)
	{
		return 1.5;
	}
	else if (concentration < CaC_10uM)
	{
		return 2;
	}

	TRI_LOG_STR(" ERROR in data_test_to_plus40mV(): " << concentration);
	assert(false); // check to see if this needs improvement
}

//return get_logarithmic_linear_proportional(CaC_log_3uM, CaC_log_10uM, 1, 2, concentration);

// This is rate: to/(to+tc)
float Generator::data_test_or_plus40mV(float concentration)
{
	if (concentration < CaC_100nM)
	{
		return 0.1;
	}
	else if (concentration < CaC_10uM)
	{
		return get_logarithmic_logarithmic_proportional(CaC_log_100nM, CaC_log_10uM, 0.1, 1000, concentration); // 300 not 1000
	}
	else
	{
		return 300;
	}
	// assert(false); // check to see if this needs improvement
}

float Generator::data_test_tc_plus40mV(float concentration)
{
	// no data available
	// assert(concentration >= CaC_log_100nM && concentration <= CaC_log_10uM);
	return get_logarithmic_logarithmic_proportional(-7, -5, -1, 3, concentration);
}
