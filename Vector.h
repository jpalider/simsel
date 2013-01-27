#ifndef SIM_POSITION_H
#define SIM_POSITION_H

//#include "Translation.h"
#include <ostream>

class Translation;

class Vector {
public:	
	float x, y, z;	

	Vector(float x, float y, float z);
	Vector();

	void translate(Translation t);

private:
        friend std::ostream & operator<<(std::ostream &os, const Vector& p);
};

#endif /* SIM_POSITION_H */
