#include <ctime>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cassert>

#include "Test.h"
#include "Math.h"
#include "Simulation.h"
#include "Generator.h"
#include "tri_logger/tri_logger.hpp"

using namespace std;

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

	//long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
	long page_size_kb = 4;
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
	double time_step = 10*1e-6;
	double r = 3e-6;
	double Q = 5e5;
	double D = 1e-9;

	for (int i = 1; i < samples; ++i)
	{
		double t = i*time_step;
		double c = concentration(r, t, D, Q);
		c *= pow(1e-6,3); // um^3
		stat_stream << t << " " << c << "\n";
	}

	stat_stream << endl;
	stat_stream.close();

}

void test_box_intersection()
{
	Vector box_begin ( 0,  0,  0);
	Vector box_end   (10, 10, 10);
	Vector line1_begin( 5,  5, -1);
	Vector line1_end  ( 5,  5, 11);
	Vector line2_begin( 5,  5,  5);
	Vector line2_end  ( 5,  5,  6);
	Vector line3_begin( 5,  5,  5);
	Vector line3_end  ( 5,  5, 15);
	Vector line4_begin( 5,  5, 11);
	Vector line4_end  ( 5,  5, 15);

	assert( segment_line_box_intersect(box_begin, box_end, line1_begin, line1_end));
	assert( segment_line_box_intersect(box_begin, box_end, line2_begin, line2_end));
	assert( segment_line_box_intersect(box_begin, box_end, line3_begin, line3_end));
	assert(!segment_line_box_intersect(box_begin, box_end, line4_begin, line4_end));
}

void test_box_intersection2()
{
	Vector box_begin  (  -5,  -5,  -5);
	Vector box_end    (   5,   5,   5);
	Vector line1_begin( -10, -10, -10);
	Vector line1_end  (  10,  10,  10);
	Vector line2_begin(  -4.9,   -4.9,  -4.9);
	Vector line2_end  (  5,   5,   5);

	assert(segment_line_box_intersect(box_begin, box_end, line1_begin, line1_end));
	assert(segment_line_box_intersect(box_begin, box_end, line2_begin, line2_end));
}



void test_log_log_proportional()
{

	// const float CaC_100nM =   1e-7;
	// const float CaC_1uM   =   1e-6;
	// //return get_logarithmic_logarithmic_proportional(-7, -5, -1, 3, concentration);

	// TRI_LOG_STR(":::" << gen::get_logarithmic_logarithmic_proportional(-7, -5, -1, 3, CaC_100nM*1) );
	// TRI_LOG_STR(":::" << gen::get_logarithmic_logarithmic_proportional(-7, -5, -1, 3, CaC_100nM*5) );
	// TRI_LOG_STR(":::" << gen::get_logarithmic_logarithmic_proportional(-7, -5, -1, 3, CaC_1uM*1) );
	// TRI_LOG_STR(":::" << gen::get_logarithmic_logarithmic_proportional(-7, -5, -1, 3, CaC_1uM*5) );
	// TRI_LOG_STR(":::" << gen::get_logarithmic_logarithmic_proportional(-7, -5, -1, 3, CaC_1uM*10) );

	return;
}

void test_sphere_containing()
{
	Receptor r1(-1, Vector(3.0, 0, 0), 1.4 );
	Molecule m1(-2, Vector(3.3, 0.71, 0.31));

	Receptor r2(-1, Vector(3.0, 0, 0), 1.4 );
	Molecule m2(-3, Vector(3.1, 1.3,  0.86));

	assert( r1.has_inside(&m1));
	assert(!r2.has_inside(&m2));
}

void test_box_containing()
{
	Receptor r1(-1, Vector(3,0,0), 0.8, 0.8, 0.8);
	Molecule m1(-2, Vector(3.39,0.32,0.31));
	Molecule m2(-3, Vector(3.3,0.21,0.15));
	Molecule m3(-4, Vector(3.41,0.32,0.31));
	assert( r1.has_inside(&m1));
	assert( r1.has_inside(&m2));
	assert(!r1.has_inside(&m3));

	Receptor r10(-1, Vector(3,0,0), 0.9, 0.9, 0.9);
	Molecule m11(-2, Vector(3.1,0.16,-0.095));
	Molecule m12(-3, Vector(3.2,-0.013,-0.2));
	assert(r10.has_inside(&m11));
	assert(r10.has_inside(&m12));
}

namespace {

template<typename T>
void print_to_file(std::string filename, std::vector<T>& data, std::vector<T>& val)
{
	ofstream po_stream;
	stringstream ss;

	ss << "test/" << filename;

	po_stream.open(ss.str().c_str(), ios::trunc);
	if (!po_stream.is_open())
	{
		assert(false);
	}

	po_stream << "# " << endl;
	for (auto v = val.begin(), d = data.begin(); v != val.end(); ++v, ++d)
	{
		po_stream << *d << " " << *v << endl;
	}

	po_stream.close();
}

std::vector<float> concentrations = {
	0.000001, 0.000002, 0.000003, 0.000004, 0.000005, // (5)    1 uM, ...,   5 uM
	0.000006, 0.000007, 0.000008, 0.000009,           // (9)    6 uM, ...,   9 uM
	0.000010, 0.000020, 0.000030, 0.000040, 0.000050, // (14)  10 uM, ...,  50 uM
	0.000060, 0.000070, 0.000080, 0.000090,           // (18)  60 uM, ...,  90 uM,
	0.000100, 0.000200, 0.000300, 0.000400, 0.000005, // (23) 100 uM, ..., 400 uM (ca. 0.5mM)
	0.000600, 0.000700, 0.000800, 0.000900,           // (27) 600 uM, ..., 900 uM
	0.001000, 0.002000, 0.003000, 0.004000, 0.00500,  // (32    1 mM, ...,   5 mM
	0.006000, 0.007000, 0.008000, 0.009000,           // (36)   6 mM, ...,   9 mM
	0.010000, 0.020000, 0.030000, 0.040000, 0.05000,  // (41)  10 mM, ...,  50 mM
};

std::vector<float> concentrations_Po(concentrations.begin(),concentrations.begin() + 23);
std::vector<float> concentrations_to(concentrations.begin(),concentrations.begin() + 9);
std::vector<float> concentrations_or(concentrations.begin(),concentrations.begin() + 9);

}

void test_Po()
{
	std::vector<float> values;

	for (auto &c : concentrations_Po)
	{
		values.push_back(Generator::data_test_Po_plus40mV(c));
	}

	assert(concentrations_Po.size() == values.size());

	print_to_file<float>("test_Po.dat", concentrations_Po, values);
}

void test_to()
{
	std::vector<float> values;

	for (auto &c : concentrations_to)
	{
		values.push_back(Generator::data_test_to_plus40mV(c));
	}

	assert(concentrations_to.size() == values.size());

	print_to_file<float>("test_to.dat", concentrations_to, values);
}

void test_or()
{
	std::vector<float> values;

	for (auto &c : concentrations_or)
	{
		values.push_back(Generator::data_test_or_plus40mV(c));
	}

	assert(concentrations_or.size() == values.size());

	print_to_file<float>("test_or.dat", concentrations_or, values);
}
