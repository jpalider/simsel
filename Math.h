#ifndef SIM_MATH_H
#define SIM_MATH_H

#include "Vector.h"

double squared_distance_between_points(const Vector* p1, const Vector* p2);

double cos_at_ab(double a2, double b2, double c2, double ab);

bool acute_angle(double cos);

Vector cross_product(const Vector* a, const Vector* b, const Vector* c);

double triangle_area(const Vector* a, const Vector* b, const Vector* c);

bool segment_line_sphere_intersect(const Vector* p1, const Vector* p2, const Vector* s, double r);

double diffusion_coefficient(double temperature_K, double viscosity_eta, double diameter);

double squared_displacement(int dimensions, double diffusion_coefficient, double tau);

double displacement_factor(int dimensions, double diffusion_coefficient, double tau);

double phi(double x);

double normal(double mean, double dev);

#endif /* SIM_MATH_H */
