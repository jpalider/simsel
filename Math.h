#ifndef SIM_MATH_H
#define SIM_MATH_H

#include "Vector.h"

float squared_distance_between_points(Vector p1, Vector p2);

float cos_at_ab(float a2, float b2, float c2, float ab);

bool acute_angle(float cos);

Vector cross_product(Vector a, Vector b, Vector c);

float triangle_area(Vector a, Vector b, Vector c);

bool segment_line_sphere_intersect(Vector p1, Vector p2, Vector s, float r);

#endif /* SIM_MATH_H */
