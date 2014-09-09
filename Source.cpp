#include <random>
#include "Source.h"
#include "Molecule.h"
#include "Conversion.h"
#include "Generator.h"
#include "tri_logger/tri_logger.hpp"

namespace
{
	float (*x_to)(float) = Generator::x_to_plus40mV;
	float (*x_tc)(float) = Generator::x_tc_plus40mV;
}

Source::Source(Id identifier, Vector position, Coordinate radius) : Boundary(identifier, position, radius)
{
	init();
}

Source::Source(Id identifier, Vector position, Coordinate size_x, Coordinate size_y, Coordinate size_z) : Boundary(identifier, position, size_x, size_y, size_z)
{
	init();
}

void Source::init()
{
	static double constexpr Ca100nM = 0.1 * 0.001 * 0.001;
	std::random_device rd;
	std::mt19937 mt_gen = std::mt19937(rd());
	std::uniform_int_distribution<int> uniform = std::uniform_int_distribution<int>(0,100);

	state = State::UNKNOWN;
	released = true; // we do not want release on simulation startup
	float offset_from_cycle_start = 100 - uniform(mt_gen);

	Time to = Conversion::ms_to_ns(x_to(Ca100nM));
	Time tc = Conversion::ms_to_ns(x_tc(Ca100nM));
	Time cycle_time = to + tc;
	cycle_start_time = 0 + offset_from_cycle_start*cycle_time/100; // time is simulation beginning = 0
	if (cycle_start_time < to)
	{
		cycle_start_time += cycle_time;
	}
	open_time = cycle_start_time - to;
	state = cycle_start_time > (0 + tc) ? State::OPEN : State::CLOSED;
	// TRI_LOG_STR(id() << " Initialized to  " << (state == State::OPEN ? "OPEN " : "CLOSED ") <<
	// 	    offset_from_cycle_start << "% ----> next cycle start=" <<
	// 	    Conversion::ns_to_ms(cycle_start_time) << " [ms]" <<
	// 	    " next open start=" << Conversion::ns_to_ms(open_time) << " [ms]" <<
	// 	    " cycle total = " << Conversion::ns_to_ms(tc+to) );
}

bool Source::run(Time time, MStore *molecules, Obstacle *space)
{
	if (state == State::OPEN)
	{
		float concentration_uM = sense(time, molecules);
		float concentration = normalize(concentration_uM) * pow(10,-6);
		if (!released)
		{
			uint count = Conversion::pA_to_molecules(20);
			released = release(time, molecules, count, space);
			return released;
		}

		if (time >= cycle_start_time)
		{
			state = State::CLOSED;
			open_time = time + Conversion::ms_to_ns(x_tc(concentration));
			cycle_start_time = closed_time = open_time + Conversion::ms_to_ns(x_to(concentration));
			// TRI_LOG_STR(id() << " Switched to CLOSED at " << Conversion::ns_to_ms(time) << "[ms]" <<
			// 	    " for " << Conversion::ns_to_ms(open_time - time) << "[ms]" );
		}

		return false;
	}
	else if (state == State::CLOSED)
	{
		released = false;
		if (time >= open_time)
		{
			state = State::OPEN;
			// TRI_LOG_STR("Switched to OPEN at " << Conversion::ns_to_ms(time) << "[ms]" <<
			// 	    " for " << Conversion::ns_to_ms(cycle_start_time - time) << "[ms]" );
		}
	}
	else {
		TRI_LOG_STR("oops, something gone wrong...");
	}

	return false;
}

bool Source::release(Time time, MStore *molecules, uint count, Obstacle *space)
{
	for (size_t i = 0; i < count; i++)
	{
		molecules->push_back(Molecule(i, cposition, space));
	}
	return true;
}

// we set up for +40mV w/o ATP
float Source::sense(Time time, MStore *molecules)
{
	uint m_count = 0;
	for (auto it = molecules->begin(); it != molecules->end(); ++it)
	{
		auto m = *it;
		if (has_inside(&m))
		{
			m_count++;
		}
	}

	float concentration = Conversion::molecules_um3_to_uM(m_count, sphere_volume(cradius));
	return concentration;
}

float Source::normalize(float concentration_uM)
{
	if (concentration_uM < 0.1 * Conversion::uM)
	{
		return 0.1 * Conversion::uM;
	}
	if (concentration_uM > 1000 * Conversion::uM)
	{
		return 1000 * Conversion::uM;
	}
	return concentration_uM;
}

bool Source::collide(Molecule *m)
{
	return false;
}

void Source::handle_collision(Molecule *m)
{

}
