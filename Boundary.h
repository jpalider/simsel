#ifndef SIM_BOUNDARY_H
#define SIM_BOUNDARY_H

#include <iostream>

#include "Vector.h"
#include "Types.h"
#include "Molecule.h"
#include "Math.h"

class Boundary;

class Collider
{
public:
	virtual bool check_collision(const Molecule *m, const Boundary *b) const = 0;
	virtual bool has_inside(Molecule *m, const Boundary *b) const = 0;
};

class Boundary
{

protected:
	Collider* collider;

	Vector cposition;
	Coordinate cradius;
	Coordinate xsize;
	Coordinate ysize;
	Coordinate zsize;
	Vector corner_b;
	Vector corner_e;

	Id cid;

public:
	Boundary(Id identifier, Vector position, Coordinate radius);
	Boundary(Id identifier, Vector position, Coordinate size_x, Coordinate size_y, Coordinate size_z);
	Boundary(const Boundary &other);

	virtual ~Boundary();
	Coordinate radius() const; // FOR REMOVAL
	const Vector* position() const;
	const Vector* corner_begin() const;
	const Vector* corner_end() const;
	Id id() const;

	/** Returns true if a given on non-transparent or shall act on molecule */
	virtual bool collide(Molecule *m) = 0;
	/** Checks collision between  molecule and this object */
	virtual bool check_collision(const Molecule *m) const;
	virtual void handle_collision(Molecule *m) = 0;
	virtual bool has_inside(Molecule *m) const;

private:
        friend std::ostream & operator<<(std::ostream &os, const Boundary& c);

	
};


	class BoxCollider : public Collider
	{
	public:
		bool check_collision(const Molecule *m, const Boundary* b) const
		{
e			return segment_line_box_intersect(*b->corner_begin(), *b->corner_end(), *m->position(), *m->prev_position());
		}
		bool has_inside(Molecule *m, const Boundary *b) const
		{
			return vector_in_box(*m->position(), *b->corner_begin(), *b->corner_end());
		}
	};

	class SphereCollider : public Collider
	{
	public:
		bool check_collision(const Molecule *m, const Boundary* b) const
		{
			return segment_line_sphere_intersect(m->position(), m->prev_position(), b->position(), b->radius());
		}
		bool has_inside(Molecule *m, const Boundary *b) const
		{
			auto r = b->radius();
			return squared_distance_between_points(b->position(), m->position()) < r*r;
		}
	};

	class ColliderFactory
	{
	public:
		static BoxCollider box_collider;
		static SphereCollider sphere_collider;
	public:
		static BoxCollider* box() { return &box_collider; }
		static SphereCollider* sphere() { return &sphere_collider; }
	};

#endif /* SIM_BOUNDARY_H */













