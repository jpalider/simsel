#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include <list>
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
	pshow = cfg.lookup("player.view.show");

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

	ptail = cfg.lookup("player.molecules.tail");

}

int Player::interval_ms()
{
	return (int)(1000/pfps);
}

static double player_scale = 1e10;

CairoColor player_string_color(222,222,222);

void Player::do_drawing_cell(cairo_t *cr, Boundary* c, Vector* origin)
{
	cairo_set_source_rgb(cr, prcolor.red(), prcolor.green(), prcolor.blue());

	int cx = player_scale * c->position()->x;
	int cy = player_scale * c->position()->y;

	cairo_identity_matrix(cr);
	cairo_translate(cr, origin->x + cx , origin->y + cy);
	cairo_arc(cr, 0, 0, prradius, 0, 2 * M_PI);
	cairo_fill(cr);

	cairo_set_source_rgb(cr, player_string_color.red(), player_string_color.green(), player_string_color.blue());
	cairo_identity_matrix(cr);

	stringstream ss;
	ss <<  "[" << c->id() << "]: " << ((RCell*)c)->molecules()->size();
	string s = ss.str();

	cairo_text_extents_t extents;
	cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size (cr, 9.0);
	cairo_text_extents (cr, s.c_str(), &extents);

	int tx = origin->x + cx - (extents.width/2 + extents.x_bearing);
	int ty = origin->y + cy - (extents.height/2 + extents.y_bearing);
	cairo_move_to (cr, tx, ty);
	cairo_show_text (cr, s.c_str());
}

void Player::do_drawing_molecule(cairo_t *cr, Molecule* m, Vector* origin, long t)
{
	const map<long, Vector>* h = m->histogram();
	map<long, Vector>::const_iterator pit = h->lower_bound(t);
	if (pit == h->end())
		return;

	cairo_set_source_rgb(cr,pmcolor.red(), pmcolor.green(), pmcolor.blue());
	cairo_identity_matrix(cr);

	int mx = player_scale * pit->second.x;
	int my = player_scale * pit->second.y;
	cairo_translate(cr, origin->x + mx, origin->y + my);
	cairo_arc(cr, 0, 0, pmradius, 0, 2 * M_PI);
	cairo_fill(cr);
}

void Player::do_drawing_molecule_with_tail(cairo_t *cr, Molecule* m, Vector* origin, long t)
{
	const map<long, Vector>* h = m->histogram();
	map<long, Vector>::const_iterator pit = h->lower_bound(t);
	if (pit == h->end())
	{
		TRI_LOG_STR("reached end of molecule histogram at " << t );
		return;
	}

	// draw molecule
	cairo_set_source_rgb(cr, pmcolor.red(), pmcolor.green(), pmcolor.blue());
	cairo_identity_matrix(cr);

	int mx = player_scale * pit->second.x;
	int my = player_scale * pit->second.y;

	cairo_translate(cr, origin->x + mx, origin->y + my);
	cairo_arc(cr, 0, 0, 3, 0, 2 * M_PI);
	cairo_fill(cr);

	// draw tail
	cairo_identity_matrix(cr);
	cairo_translate(cr, origin->x, origin->y);

	cairo_set_line_width(cr, 1);
	cairo_set_source_rgb(cr, 0 , 0.69, 0);

	std::map<long, Vector>::const_iterator phit = h->begin();
	std::map<long, Vector>::const_iterator hit = ++h->begin();

	for ( ; hit != h->end(); ++hit, ++phit)
	{
		int ptx = player_scale * phit->second.x;
		int pty = player_scale * phit->second.y;
		int tx = player_scale * hit->second.x;
		int ty = player_scale * hit->second.y;

		cairo_move_to(cr, ptx, pty);
		cairo_line_to(cr, tx, ty);
		cairo_stroke(cr);
		if (hit == pit)
			break;
	}

}

void Player::do_drawing(cairo_t *cr, GtkWidget* widget)
{
	//TRI_LOG_STR("do_drawing");

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
	for (vector<RCell>::iterator it = psimulation->receivers()->begin(); it != psimulation->receivers()->end(); ++it)
	{
		do_drawing_cell(cr, &(*it), &origin);
	}

	// draw molecules
	if (ptail)
	{
		for (list<Molecule*>::iterator it = psimulation->molecules()->begin(); it != psimulation->molecules()->end(); ++it)
		{
			do_drawing_molecule_with_tail(cr, (*it), &origin, ptime);
		}
	}
	else
	{
		for (list<Molecule*>::iterator it = psimulation->molecules()->begin(); it != psimulation->molecules()->end(); ++it)
		{
			do_drawing_molecule(cr, (*it), &origin, ptime);
		}
	}

	//UNDER DEV: show only those molecules and tails that collide with rcells
	// for (vector<RCell>::iterator rit = psimulation->receivers()->begin(); rit != psimulation->receivers()->end(); ++rit)
	// {
	// 	for (vector<Molecule*>::iterator it = rit->molecules()->begin(); it != rit->molecules()->end(); ++it)
	// 	{
	// 		do_drawing_molecule_with_tail(cr, (*it), &origin, ptime);
	// 	}
	// }

}

bool Player::show()
{
	return pshow;
}
