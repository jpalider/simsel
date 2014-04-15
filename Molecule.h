#ifndef SIM_MOLECULE_H
#define SIM_MOLECULE_H

#include <map>
#include <iostream>

#include "Types.h"
#include "Vector.h"
#include "Boundary.h"

class Molecule {
private:
	Vector                 mposition;
	Vector                 mpposition;
	Coordinate             mradius;
	Id                     mid;
	Boundary*              mcell;
	void *                 mowner;

public:
	Molecule(Id identifier, Vector position, void * owner = nullptr);

	/** Tells currect object position */
	const Vector* position() const;
	/** Translate vector */
	void move(const Vector t);
	/** Restore position of last move() operation */
	void move_back();
	/** Checks collision between this molecule and another Boundary object */
	bool check_collision(const Boundary *c);
	/** Molecule can be free or bound or even destroyed - owner denotes where it is*/
	bool is_owner(void * owner);
	/** Moving from one object into another */
	void change_owner(void * owner);
private:
	friend std::ostream & operator<<(std::ostream &os, const Molecule& m);

};

#endif /* SIM_MOLECULE_H */
