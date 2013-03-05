#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#include "Test.h"
#include "Math.h"
#include "Simulation.h"
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

	// TP_Chapter 2 : nitrogen
	// expected result: 1.15e-9
	{
		double temperature_K = 293.;
		double viscosity_eta = 1e-3;
		double diameter = 0.75e-10; // nitrogen

		double D = diffusion_coefficient(temperature_K, viscosity_eta, diameter);
		TRI_LOG_STR("diffusion_coefficient = " << D);
	}

	// TP_Chapter 2 : hydrogen
	// expected result: 3.4e-9
	{
		double temperature_K = 293.;
		double viscosity_eta = 1e-3;
		double diameter = 1.06e-10; // nitrogen

		double D = diffusion_coefficient(temperature_K, viscosity_eta, diameter);
		TRI_LOG_STR("diffusion_coefficient = " << D);
	}

	return;
}


void test_memory_usage_print()
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

// https://bbs.archlinux.org/viewtopic.php?id=79378
static std::streambuf * cout_sbuf = NULL;

#define enable_cout() \
do { \
	std::cout.rdbuf(cout_sbuf); \
} while(0);

#define disable_cout() \
do { \
	cout_sbuf = std::cout.rdbuf(); \
	std::ofstream fout("/dev/null"); \
	std::cout.rdbuf(fout.rdbuf()); \
} while(0);

void test_memory_usage()
{
	disable_cout();
	Simulation s;
	s.run();
	enable_cout();

	test_memory_usage_print();
	disable_cout();
	for (int i = 0; i < 100; i++)
	{
		Simulation s;
		s.run();
	}
	enable_cout();

	test_memory_usage_print();
}

double concentration(double r, double t, double d, double q)
{
	return q / pow(4*M_PI*d*t, 1.5) * exp(-1*r*r/(4*d*t));
}

void test_diffusion_equation()
{
	ofstream stat_stream;
	stat_stream.open((string("results/") + string("test_diff_eq") + string(".txt")).c_str(), ios::trunc);

	int samples = 100000;
	double time_step = 1e-6;
	double r = 3e-6;
	double Q = 5e5;
	double D = 1e-9;

	for (int i = 1; i < samples; ++i)
	{
		double t = i*time_step;
		double c = concentration(r, t, D, Q);
		c *= pow(1e-6,3);
		stat_stream << t << " " << c << "\n";
	}

	stat_stream << endl;
	stat_stream.close();

}
