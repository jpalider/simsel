#ifndef SIM_SOURCE_H
#define SIM_SOURCE_H

#include <vector>

#include "Boundary.h"
#include "Obstacle.h"
#include "Types.h"
#include "Vector.h"

class Molecule;

/**
 * State of a source, UNKNOWN is not really used at the moment
 */
enum class State
{
	UNKNOWN,
	OPEN,
	CLOSED
};

/**
 * Repesents source of Ca2+
 *
 */
class Source : public Boundary
{
private:
	State state;
	Time cycle_start_time;    // when the sensing and releasing cycle start
	Time open_time;
	Time closed_time;
	bool released;

	void init();
public:
	Source(Id identifier, Vector position, Coordinate radius);
	Source(Id identifier, Vector position, Coordinate size_x, Coordinate size_y, Coordinate size_z);
	bool collide(Molecule *m);
	void handle_collision(Molecule *m);

	virtual bool run(long time, MStore *molecules, Obstacle *space = nullptr);
	virtual bool release(long time, MStore *molecules, uint count, Obstacle *space = nullptr);
	virtual float sense(Time time, MStore *molecules);
	virtual float normalize(float concentration_uM);
};

#endif /* SIM_SOURCE_H */
