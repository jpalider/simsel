#include <cmath>
#include <map>
#include <vector>

#include <cairo.h>
#include <gtk/gtk.h>

#include "Player.h"

using namespace std;

Player::Player(Simulation* simulation)
{
	cout << "Player" << endl;
	pmcolor = CairoColor(0.79f, 0.39f, 0.19f);
	pccolor = CairoColor(0.19, 0.69, 0);
	psimulation = simulation;
	pfps = 10;
	ptime = 0;
}

int Player::interval_ms()
{
	return (int)(1000/pfps);
}

void Player::do_drawing_cell(cairo_t *cr, Cell* c, Vector* origin)
{
	cairo_set_source_rgb(cr, pccolor.red(), pccolor.green(), pccolor.blue());

	cairo_identity_matrix(cr);
	cairo_translate(cr, origin->x + c->position()->x, origin->y + c->position()->y);
	cairo_arc(cr, 0, 0, c->radius(), 0, 2 * M_PI);
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
	cairo_arc(cr, 0, 0, 3, 0, 2 * M_PI);
	cairo_fill(cr);
}

void Player::do_drawing_molecule_with_tail_at(cairo_t *cr, Molecule* m, Vector* origin, long t)
{
	const map<long, Vector>* h = m->histogram();
	map<long, Vector>::const_iterator pit = h->lower_bound(t);
	if (pit == h->end())
	{
		cout << "reached end of molecule histogram at " << t << endl;
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
	ptime += 1;
	cout << "do_drawing" << endl;
	
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
	for (vector<Cell>::iterator it = psimulation->receivers()->begin(); it != psimulation->receivers()->end(); it++)
	{
		do_drawing_cell(cr, &(*it), &origin);
	}

	// draw molecules
	for (vector<Molecule>::iterator it = psimulation->molecules()->begin(); it != psimulation->molecules()->end(); it++)
	{
		do_drawing_molecule_with_tail_at(cr, &(*it), &origin, ptime);
	}
}
