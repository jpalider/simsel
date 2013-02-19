#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>

#include "Test.h"
#include "Math.h"
#include "tri_logger/tri_logger.hpp"

using namespace std;

void test_normal_distribution()
{
//	srand ( time(NULL) );

	const int nrolls=10000;  // number of experiments
	const int nstars=100;    // maximum number of stars to distribute

	int pr[10]={};
//	srand ( time(NULL) );


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

void test_diffusion_coefficient()
{
	// from SIMULATION BROWNIAN MOTION
	// expected result: 4.2902e-013

	{
		double temperature_K = 293.;
		double viscosity_eta = 1e-3;
		double diameter = 1e-6;

		double D = diffusion_coefficient(temperature_K, viscosity_eta, diameter);
		TRI_LOG_STR("diffusion_coefficient = " << D);
	}

	// TP_Chapter 2
	// expected result: 1.15e-9
	{
		double temperature_K = 293.;
		double viscosity_eta = 1e-3;
		double diameter = 0.75e-10; // nitrogen

		double D = diffusion_coefficient(temperature_K, viscosity_eta, diameter);
		TRI_LOG_STR("diffusion_coefficient = " << D);
	}

	// TP_Chapter 2
	// expected result: 3.4e-9
	{
		double temperature_K = 293.;
		double viscosity_eta = 1e-3;
		double diameter = 0.75e-10; // nitrogen

		double D = diffusion_coefficient(temperature_K, viscosity_eta, diameter);
		TRI_LOG_STR("diffusion_coefficient = " << D);
	}

	return;
}

void test_memory_usage()
{
	//http: stackoverflow.com/questions/669438/how-to-get-memory-usage-at-run-time-in-c
	int tSize = 0, resident = 0, share = 0;
	ifstream buffer("/proc/self/statm");
	buffer >> tSize >> resident >> share;
	buffer.close();

	long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
	double rss = resident * page_size_kb;
	cout << "RSS - " << rss << " kB\n";

	double shared_mem = share * page_size_kb;
	cout << "Shared Memory - " << shared_mem << " kB\n";

	cout << "Private Memory - " << rss - shared_mem << "kB\n";
}
