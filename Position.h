#ifndef SIM_POSITION_H
#define SIM_POSITION_H

//#include "Translation.h"
#include <ostream>

class Translation;

class Position {
public:	
	float x, y, z;	

	Position(float x, float y, float z);
	Position();

	void translate(Translation t);

private:
        friend std::ostream & operator<<(std::ostream &os, const Position& p);
};

#endif /* SIM_POSITION_H */
