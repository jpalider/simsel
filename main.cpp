/**
 * TODO:
 * - releasing molecule not a point but a sphere
 * + add sender/recevier cell
 * - serialising (protocol buffer?)
 * - configuration
 * - logging
 * - test / verification
 * - simulation and gui shall be separated entities
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

#include <cairo.h>
#include <gtk/gtk.h>

#include "Molecule.h"
#include "Vector.h"
#include "BrownianMotion.h"
#include "Simulation.h"
#include "Cell.h"

using namespace std;

int NUMBER_OF_MOLECULES = 10;
int NUMBER_OF_ITERATIONS = 50;

vector<Molecule> molecules;
vector<Cell> transmit_cells;
vector<Cell> receive_cells;

static CairoColor mcolor(0.79f, 0.39f, 0.19f);
static CairoColor ccolor(0.19, 0.69, 0);

Simulation* s;
long vistime = 0;


void do_drawing(cairo_t *, GtkWidget* widget);

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{      
  cr = gdk_cairo_create(gtk_widget_get_window(widget));
  do_drawing(cr, widget);
  cairo_destroy(cr);

  return FALSE;
}


gboolean
time_handler(GtkWidget *widget)
{
	cout << vistime << " " << s->time() << endl;
	if (!s->started())
	{
		return true;
	}

	if (vistime > s->time())
	{
		if (s->finished())
		{
			return false;
		}
		return true;
	}

	vistime++;
	//time++;
	gtk_widget_queue_draw(widget);
	return true;
}

void do_drawing_cell(cairo_t *cr, Cell* c, Vector* origin)
{
	cairo_set_source_rgb(cr, ccolor.red(), ccolor.green(), ccolor.blue());

	cairo_identity_matrix(cr);
	cairo_translate(cr, origin->x + c->position()->x, origin->y + c->position()->y);
	cairo_arc(cr, 0, 0, c->radius(), 0, 2 * M_PI);
	cairo_fill(cr);

}

void do_drawing_molecule_at(cairo_t *cr, Molecule* m, Vector* origin, long t)
{
	const map<long, Vector>* h = m->histogram();
	map<long, Vector>::const_iterator pit = h->lower_bound(t);
	if (pit == h->end())
		return;

	cairo_set_source_rgb(cr,mcolor.red(), mcolor.green(), mcolor.blue());

	cairo_identity_matrix(cr);
	cairo_translate(cr, origin->x + pit->second.x, origin->y + pit->second.y);
	cairo_arc(cr, 0, 0, 3, 0, 2 * M_PI);
	cairo_fill(cr);
}

void do_drawing_molecule_with_tail_at(cairo_t *cr, Molecule* m, Vector* origin, long t)
{
	const map<long, Vector>* h = m->histogram();
	map<long, Vector>::const_iterator pit = h->lower_bound(t);
	if (pit == h->end())
	{
		cout << "reached end of molecule histogram" << endl;
		return;
	}
	cairo_set_source_rgb(cr, mcolor.red(), mcolor.green(), mcolor.blue());

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


void do_drawing(cairo_t *cr, GtkWidget* widget)
{
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
	for (vector<Cell>::iterator it = receive_cells.begin(); it != receive_cells.end(); it++) {
		do_drawing_cell(cr, &(*it), &origin);
	}

	// draw molecules
	for (vector<Molecule>::iterator it = molecules.begin(); it != molecules.end(); it++) {
		do_drawing_molecule_with_tail_at(cr, &(*it), &origin, vistime);
	}
}


static gpointer thread_func( gpointer data )
{
	s->run(NUMBER_OF_MOLECULES, NUMBER_OF_ITERATIONS, &molecules, &receive_cells);
	return NULL;
}


int main(int argc, char **argv) {
	cout << "Welcome to the Sim" << endl;

	if (argc > 1)
	{
		NUMBER_OF_MOLECULES = atoi(argv[1]);
	}
	
	if (argc > 2)
	{
		NUMBER_OF_ITERATIONS = atoi(argv[2]);;
	}

	if( ! g_thread_supported() )
		g_thread_init( NULL );

	gdk_threads_init();

	gdk_threads_enter();
	
	GtkWidget *window;
	GtkWidget *darea;
	GThread *thread;
	GError *error = NULL;
	
	gtk_init(&argc, &argv);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	darea = gtk_drawing_area_new();
	gtk_container_add(GTK_CONTAINER(window), darea);

	g_signal_connect(G_OBJECT(darea), "draw", 
			 G_CALLBACK(on_draw_event), NULL); 
	g_signal_connect(window, "destroy",
			 G_CALLBACK(gtk_main_quit), NULL);

	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);
	gtk_window_set_title(GTK_WINDOW(window), "Sim");

	g_timeout_add(10*300, (GSourceFunc) time_handler, (gpointer) window);


	s = new Simulation();
	thread = g_thread_create( thread_func, NULL, FALSE, &error );
	if( ! thread )
	{
		g_print( "Error: %s\n", error->message );
		return( -1 );
	}

	
	
	gtk_widget_show_all(window);

	gtk_main();

	gdk_threads_leave();


	return 0;
}

