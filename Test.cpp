#include <ctime>
#include <cstdlib>
#include <string>

#include "Test.h"
#include "Math.h"
#include "tri_logger/tri_logger.hpp"

void test_normal_distribution()
{
	srand ( time(NULL) );

	const int nrolls=10000;  // number of experiments
	const int nstars=100;    // maximum number of stars to distribute

	int pr[10]={};
	srand ( time(NULL) );


	for (int i=0; i<nrolls; ++i) {
		float number = normal(2, 5);
		++pr[int(number)];
	}

	TRI_LOG_STR("normal_distribution (5.0,2.0):");

	for (int i=0; i<10; ++i) {
		TRI_LOG_STR(i << "-" << (i+1) << ": ");
		TRI_LOG_STR(std::string(pr[i]*nstars/nrolls,'*'));
	}

	return;
}
