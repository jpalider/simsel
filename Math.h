#ifndef SIM_MATH_H
#define SIM_MATH_H

#include "Vector.h"

float squared_distance_between_points(const Vector* p1, const Vector* p2);

float cos_at_ab(float a2, float b2, float c2, float ab);

bool acute_angle(float cos);

Vector cross_product(const Vector* a, const Vector* b, const Vector* c);

float triangle_area(const Vector* a, const Vector* b, const Vector* c);

bool segment_line_sphere_intersect(const Vector* p1, const Vector* p2, const Vector* s, float r);

float diffusion_coefficient(float temperature_K, float viscosity_eta, float diameter);

float squared_displacement(int dimensions, float diffusion_coefficient, float tau);

#endif /* SIM_MATH_H */
