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
	virtual bool check_collision(const Molecule *m, const Boundary *b) = 0;
	virtual bool has_inside(Molecule *m, const Boundary *b) = 0;
};

class Boundary
{

private:
	Collider* collider;

protected:
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
	virtual bool check_collision(const Molecule *m);
	virtual void handle_collision(Molecule *m) = 0;
	virtual bool has_inside(Molecule *m);

private:
        friend std::ostream & operator<<(std::ostream &os, const Boundary& c);

	
};


	class BoxCollider : public Collider
	{
	public:
		bool check_collision(const Molecule *m, const Boundary* b)
		{
			return segment_line_box_intersect(*m->position(), *m->prev_position(), *b->corner_begin(), *b->corner_end());
		}
		bool has_inside(Molecule *m, const Boundary *b) { return false; }
	};

	class SphereCollider : public Collider
	{
	public:
		bool check_collision(const Molecule *m, const Boundary* b)
		{
			return segment_line_sphere_intersect(m->position(), m->prev_position(), b->position(), b->radius());
		}
		bool has_inside(Molecule *m, const Boundary *b) { return false; }
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













