#include <cmath>
#include <map>
#include <vector>
#include <libconfig.h++>

#include <cairo.h>
#include <gtk/gtk.h>

#include "Player.h"
#include "tri_logger/tri_logger.hpp"

using namespace std;

Player::Player(Simulation* simulation)
{
	TRI_LOG_STR("Player");
	psimulation = simulation;
	ptime = 0;
	cfg.readFile("cfg/Player.cfg");

	pfps = cfg.lookup("player.view.fps");
	pmcolor = CairoColor(cfg.lookup("player.molecules.color.red"),
			     cfg.lookup("player.molecules.color.green"),
			     cfg.lookup("player.molecules.color.blue"));
	pmccolor = CairoColor(cfg.lookup("player.molecules.color_collided.red"),
			      cfg.lookup("player.molecules.color_collided.green"),
			      cfg.lookup("player.molecules.color_collided.blue"));
	pmtcolor = CairoColor(cfg.lookup("player.molecules.color_tail.red"),
			      cfg.lookup("player.molecules.color_tail.green"),
			      cfg.lookup("player.molecules.color_tail.blue"));
	ptcolor = CairoColor(cfg.lookup("player.transmitters.color.red"),
			     cfg.lookup("player.transmitters.color.green"),
			     cfg.lookup("player.transmitters.color.blue"));
	prcolor = CairoColor(cfg.lookup("player.receivers.color.red"),
			     cfg.lookup("player.receivers.color.green"),
			     cfg.lookup("player.receivers.color.blue"));

	pmradius = cfg.lookup("player.molecules.radius");
	ptradius = cfg.lookup("player.transmitters.radius");
	prradius = cfg.lookup("player.receivers.radius");;

	TRI_LOG_STR("Player:");
}

int Player::interval_ms()
{
	return (int)(1000/pfps);
}

void Player::do_drawing_cell(cairo_t *cr, Cell* c, Vector* origin)
{
	cairo_set_source_rgb(cr, prcolor.red(), prcolor.green(), prcolor.blue());

	cairo_identity_matrix(cr);
	cairo_translate(cr, origin->x + c->position()->x, origin->y + c->position()->y);
	cairo_arc(cr, 0, 0, prradius, 0, 2 * M_PI);
	cairo_fill(cr);
}

void Player::do_drawing_molecule_at(cairo_t *cr, Molecule* m, Vector* origin, long t)
{
	const map<long, Vector>* h = m->histogram();
	map<long, Vector>::const_iterator pit = h->lower_bound(t);
	if (pit == h->end())
		return;

	cairo_set_source_rgb(cr,pmcolor.red(), pmcolor.green(), pmcolor.blue());

	cairo_identity_matrix(cr);
	cairo_translate(cr, origin->x + pit->second.x, origin->y + pit->second.y);
	cairo_arc(cr, 0, 0, pmradius, 0, 2 * M_PI);
	cairo_fill(cr);
}

void Player::do_drawing_molecule_with_tail_at(cairo_t *cr, Molecule* m, Vector* origin, long t)
{
	const map<long, Vector>* h = m->histogram();
	map<long, Vector>::const_iterator pit = h->lower_bound(t);
	if (pit == h->end())
	{
		TRI_LOG_STR("reached end of molecule histogram at " << t );
		return;
	}

	cairo_set_source_rgb(cr, pmcolor.red(), pmcolor.green(), pmcolor.blue());

	cairo_identity_matrix(cr);
	cairo_translate(cr, origin->x + pit->second.x, origin->y + pit->second.y);
	cairo_arc(cr, 0, 0, 3, 0, 2 * M_PI);
	cairo_fill(cr);

	cairo_identity_matrix(cr);
	cairo_translate(cr, origin->x, origin->y);

	cairo_set_line_width(cr, 1);
	cairo_set_source_rgb(cr, 0 , 0.69, 0);

	std::map<long, Vector>::const_iterator phit = h->begin();
	std::map<long, Vector>::const_iterator hit = ++h->begin();

	for ( ; hit != h->end(); hit++, phit++)
	{
		cairo_move_to(cr, phit->second.x , phit->second.y);
		cairo_line_to(cr, hit->second.x , hit->second.y);
		cairo_stroke(cr);
		if (hit == pit)
			break;
	}

}

void Player::do_drawing(cairo_t *cr, GtkWidget* widget)
{
	TRI_LOG_STR("do_drawing");

	ptime += 1;
	
	GtkWidget *win = gtk_widget_get_toplevel(widget);
  
	int width, height;
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);

	Vector origin(width/2, height/2, 0);

	cairo_set_line_width(cr, 3); 
	cairo_set_source_rgb(cr, 0 ,0, 0.69);
	cairo_translate(cr, origin.x, origin.y);
	cairo_arc(cr, 0, 0, 2, 0, 2 * M_PI);
	cairo_fill(cr);

	// draw static cells
	for (vector<RCell>::iterator it = psimulation->receivers()->begin(); it != psimulation->receivers()->end(); it++)
	{
		do_drawing_cell(cr, &(*it), &origin);
	}

	// draw molecules
	for (vector<Molecule>::iterator it = psimulation->molecules()->begin(); it != psimulation->molecules()->end(); it++)
	{
		do_drawing_molecule_with_tail_at(cr, &(*it), &origin, ptime);
	}
}
