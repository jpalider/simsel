#ifndef SIM_MATH_H
#define SIM_MATH_H

#include "Vector.h"

Coordinate squared_distance_between_points(const Vector* p1, const Vector* p2);

Coordinate cos_at_ab(Coordinate a2, Coordinate b2, Coordinate c2, Coordinate ab);

bool acute_angle(double cos);

Vector cross_product(const Vector* a, const Vector* b, const Vector* c);

Coordinate triangle_area(const Vector* a, const Vector* b, const Vector* c);

bool segment_line_sphere_intersect(const Vector* p1, const Vector* p2, const Vector* s, Coordinate r);

double diffusion_coefficient(double temperature_K, double viscosity_eta, double diameter);

double squared_displacement(int dimensions, double diffusion_coefficient, double tau);

double displacement_factor(int dimensions, double diffusion_coefficient, double tau);

double phi(double x);

double normal(double mean, double dev);

Coordinate sphere_volume(double radius);

#endif /* SIM_MATH_H */
