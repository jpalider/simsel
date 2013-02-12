#ifndef SIM_VECTOR_H
#define SIM_VECTOR_H

#include <ostream>

class Vector {
public:	
	double x, y, z;

	Vector(double x, double y, double z);
	Vector();

	void translate(Vector* t);

	Vector& operator+=(const Vector& v);

private:
        friend std::ostream & operator<<(std::ostream &os, const Vector& p);
};

#endif /* SIM_VECTOR_H */
