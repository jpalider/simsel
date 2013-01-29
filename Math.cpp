#include <cmath>
#include <iostream>

#include "Math.h"
#include "Vector.h"

using namespace std;

Vector vzero(0,0,0);

float squared_distance_between_points(const Vector* p1, const Vector* p2)
{
	return (p2->x-p1->x)*(p2->x-p1->x) + (p2->y-p1->y)*(p2->y-p1->y) + (p2->z-p1->z)*(p2->z-p1->z);
}

float cos_at_ab(float a2, float b2, float c2, float ab)
{
	return (a2 + b2 - c2) / (2 * ab);
}

// Works for 0-180
bool acute_angle(float cos)
{
	return cos > 0;
}

Vector cross_product(const Vector* a, const Vector* b, const Vector* c)
{
	Vector v(a->x - c->x, a->y - c->y, a->z - c->z);
	Vector w(b->x - c->x, b->y - c->y, b->z - c->z);
		
	return Vector(v.y*w.z - w.y*v.z, w.x*v.z - v.x*w.z, v.x*w.y - w.x*v.y);
}

// http://kb->komires->net/article->php?id=2
// http://pl->wikipedia->org/wiki/Iloczyn_wektorowy
// http://www->matematyka->pl/47774->htm
float triangle_area(const Vector* a, const Vector* b, const Vector* c)
{
	Vector axb = cross_product(a, b, c);
	//cout << a << b << c << axb;
	float area = 0.5f * sqrt( squared_distance_between_points(&vzero, &axb) );
	return area;		
}

// http://stackoverflow->com/questions/2062286/testing-whether-a-line-segment-intersects-a-sphere
// -> http://www->matematyka->pl/250033->htm

/**
 * Returns true on intersection 
 */
bool segment_line_sphere_intersect(const Vector* p1, const Vector* p2, const Vector *s, float r)
{	
	if (p1 == p2)
	{
		// if ( squared_distance_between_points(p1, s) <= r*r) 
		//     cout << "intersection. = " << endl;
		return squared_distance_between_points(p1, s) <= r*r;
	}	

	float sq_p1_to_s = squared_distance_between_points(p1, s);
	float sq_p2_to_s = squared_distance_between_points(p2, s);
	float sq_p1_to_p2 = squared_distance_between_points(p1, p2);
	float p1_to_p2 = sqrt( sq_p1_to_p2 );
	float p1_to_s = sqrt( sq_p1_to_s );
	float p2_to_s = sqrt( sq_p2_to_s );


	float cos_p1 = cos_at_ab(sq_p1_to_s, sq_p1_to_p2, sq_p2_to_s, p1_to_p2 * p1_to_s );
	float cos_p2 = cos_at_ab(sq_p2_to_s, sq_p1_to_p2, sq_p1_to_s, p1_to_p2 * p2_to_s );

	 // cout << "dbg" << cos_p1 << " " << acos(cos_p1) * 360 / 2 / M_PI << " " << acute_angle(cos_p1) << endl;
	 // cout << "dbg" << cos_p2 << " " << acos(cos_p2) * 360 / 2 / M_PI << " " << acute_angle(cos_p2) << endl;

	// maybe move this before 
	if ( acute_angle(cos_p1) )
	{
		if (sq_p1_to_s < r*r)
		{
			//cout << "intersection.. = " << endl;
			return true;
		}
	}

	if ( acute_angle(cos_p2) )
	{
		if (sq_p2_to_s < r*r) {
			//cout << "intersection... = " << endl;
			return true;
		}
	}
	
	if ( acute_angle(cos_p1) && acute_angle(cos_p2) )
	{
		// procedure for intersection
		if ( 2 * triangle_area(p1, p2, s) / p1_to_p2 < r)
		{
			//cout << "intersection.... = " << endl;
			return true;
		}
	}
	
	return false;
}

float diffusion_coefficient(float temperature_K, float viscosity_eta, float diameter)
{
	static float k_b = 1.38E-10;
	
	return k_b * temperature_K / ( 3 * M_PI * diameter * viscosity_eta);
}

float squared_displacement(int dimensions, float diffusion_coefficient, float tau)
{
	return 2 * dimensions * diffusion_coefficient * tau;
}
