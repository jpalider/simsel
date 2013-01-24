#ifndef SIM_POSITION_H
#define SIM_POSITION_H

//#include "Translation.h"
class Translation;

class Position {
public:	
	float x, y, z;	

	Position(float x, float y, float z);
	Position();

	void translate(Translation t);
};

#endif /* SIM_POSITION_H */
