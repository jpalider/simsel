#include <cmath>
#include <iostream>
#include <cstdlib>

#include "Math.h"
#include "Vector.h"
#include "SimpleRNG.h"

using namespace std;

Vector vzero(0,0,0);

double squared_distance_between_points(const Vector* p1, const Vector* p2)
{
	return (p2->x-p1->x)*(p2->x-p1->x) + (p2->y-p1->y)*(p2->y-p1->y) + (p2->z-p1->z)*(p2->z-p1->z);
}

double cos_at_ab(double a2, double b2, double c2, double ab)
{
	return (a2 + b2 - c2) / (2 * ab);
}

// Works for 0-180
bool acute_angle(double cos)
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
double triangle_area(const Vector* a, const Vector* b, const Vector* c)
{
	Vector axb = cross_product(a, b, c);
	//cout << a << b << c << axb;
	double area = 0.5f * sqrt( squared_distance_between_points(&vzero, &axb) );
	return area;		
}

// http://stackoverflow->com/questions/2062286/testing-whether-a-line-segment-intersects-a-sphere
// -> http://www->matematyka->pl/250033->htm

/**
 * Returns true on intersection 
 */
bool segment_line_sphere_intersect(const Vector* p1, const Vector* p2, const Vector *s, double r)
{	
	if (p1 == p2)
	{
		// if ( squared_distance_between_points(p1, s) <= r*r) 
		//     cout << "intersection. = " << endl;
		return squared_distance_between_points(p1, s) <= r*r;
	}	

	double sq_p1_to_s = squared_distance_between_points(p1, s);
	double sq_p2_to_s = squared_distance_between_points(p2, s);
	double sq_p1_to_p2 = squared_distance_between_points(p1, p2);
	double p1_to_p2 = sqrt( sq_p1_to_p2 );
	double p1_to_s = sqrt( sq_p1_to_s );
	double p2_to_s = sqrt( sq_p2_to_s );


	double cos_p1 = cos_at_ab(sq_p1_to_s, sq_p1_to_p2, sq_p2_to_s, p1_to_p2 * p1_to_s );
	double cos_p2 = cos_at_ab(sq_p2_to_s, sq_p1_to_p2, sq_p1_to_s, p1_to_p2 * p2_to_s );

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

double diffusion_coefficient(double temperature_K, double viscosity_eta, double diameter)
{
	static double k_b = 1.38E-23;
	
	return k_b * temperature_K / ( 3 * M_PI * viscosity_eta * diameter);
}

double squared_displacement(int dimensions, double diffusion_coefficient, double tau)
{
	return 2 * dimensions * diffusion_coefficient * tau;
}

double displacement_factor(int dimensions, double diffusion_coefficient, double tau)
{
	return sqrt(squared_displacement(dimensions, diffusion_coefficient, tau));
}


//http://www.johndcook.com/cpp_phi.html
double phi(double x)
{
    // constants
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;

    // Save the sign of x
    int sign = 1;
    if (x < 0)
        sign = -1;
    x = fabs(x)/sqrt(2.0);

    // A&S formula 7.1.26
    double t = 1.0/(1.0 + p*x);
    double y = 1.0 - (((((a5*t + a4)*t) + a3)*t + a2)*t + a1)*t*exp(-x*x);

    return 0.5*(1.0 + sign*y);
}

// approximation
//http://www.protonfish.com/random.shtml
// double normal(double mean, double dev)
// {
// 	double x = (((double)rand()/RAND_MAX) * 2 - 1) + (((double)rand()/RAND_MAX) * 2 - 1) + (((double)rand()/RAND_MAX) * 2 - 1);
// 	return dev * x + mean;
// }

double normal(double mean, double dev)
{
	static SimpleRNG srng;
	return srng.GetNormal(mean, dev);
}
