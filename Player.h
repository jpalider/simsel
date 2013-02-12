#ifndef SIM_PLAYER_H
#define SIM_PLAYER_H

#include <cairo.h>
#include <libconfig.h++>
#include <gtk/gtk.h>

#include "CairoColor.h"
#include "Molecule.h"
#include "Cell.h"
#include "Simulation.h"
#include "Vector.h"

class Player
{
private:

	bool pshow;

	CairoColor pmcolor;
	CairoColor pmccolor;
	CairoColor pmtcolor;
	CairoColor prcolor;
	CairoColor ptcolor;

	int pmradius;
	int ptradius;
	int prradius;

	bool ptail;

	Simulation* psimulation;
	long ptime;
	float pfps;
	libconfig::Config cfg;

public:
	Player(Simulation* simulation);
	void do_drawing_cell(cairo_t *cr, Cell* c, Vector* origin);
	void do_drawing_molecule(cairo_t *cr, Molecule* m, Vector* origin, long t);
	void do_drawing_molecule_with_tail(cairo_t *cr, Molecule* m, Vector* origin, long t);
	void do_drawing(cairo_t *cr, GtkWidget* widget);
	int interval_ms();

	bool show();
};

#endif /* SIM_PLAYER_H */
