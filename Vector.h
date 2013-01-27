#ifndef SIM_VECTOR_H
#define SIM_VECTOR_H

#include <ostream>

class Vector {
public:	
	float x, y, z;	

	Vector(float x, float y, float z);
	Vector();

	void translate(Vector* t);

private:
        friend std::ostream & operator<<(std::ostream &os, const Vector& p);
};

#endif /* SIM_VECTOR_H */
