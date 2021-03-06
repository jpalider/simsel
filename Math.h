#ifndef SIM_MATH_H
#define SIM_MATH_H

#include "Vector.h"
#include "Types.h"

Coordinate squared_distance_between_points(const Vector* p1, const Vector* p2);

Coordinate cos_at_ab(const Coordinate a2, const Coordinate b2, const Coordinate c2, const Coordinate ab);

bool acute_angle(const double cos);

Vector cross_product(const Vector* a, const Vector* b, const Vector* c);

Coordinate triangle_area(const Vector* a, const Vector* b, const Vector* c);

bool segment_line_sphere_intersect(const Vector* p1, const Vector* p2, const Vector* s, const Coordinate r);

bool vector_in_box(const Vector &v, const Vector &b1, const Vector &b2);

bool segment_line_box_intersect(const Vector &b1, const Vector &b2, const Vector &l1, const Vector &l2);

double diffusion_coefficient(double temperature_K, double viscosity_eta, double diameter);

double squared_displacement(int dimensions, double diffusion_coefficient, double tau);

double displacement_factor(int dimensions, double diffusion_coefficient, double tau);

double phi(double x);

Coordinate sphere_volume(double radius);

#endif /* SIM_MATH_H */
