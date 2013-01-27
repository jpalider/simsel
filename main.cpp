#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

#include <cairo.h>
#include <gtk/gtk.h>

#include "Molecule.h"
#include "Vector.h"
#include "BrownianMotion.h"

using namespace std;

int NUMBER_OF_MOLECULES = 10;
int NUMBER_OF_ITERATIONS = 50;

vector<Molecule> molecules;


static void do_drawing(cairo_t *, GtkWidget* widget);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{      
  cr = gdk_cairo_create(gtk_widget_get_window(widget));
  do_drawing(cr, widget);
  cairo_destroy(cr);

  return FALSE;
}

static gboolean
time_handler(GtkWidget *widget)
{
	static int counter = 0;	
//	if (widget->window == NULL) return FALSE;
	gtk_widget_queue_draw(widget);
	if (counter++ < NUMBER_OF_ITERATIONS-2)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

static void do_drawing_molecule_at(cairo_t *cr, Molecule* m, Vector* origin, long t)
{
//	Vector p = m->get_histogram().at(t);
	map<long, Vector>h = m->histogram();
	map<long, Vector>::iterator pit = h.find(t);
	if (pit == h.end())
		return;
	
	cairo_set_source_rgb(cr,m->color()->red(), m->color()->green(), m->color()->blue());

	cairo_identity_matrix(cr);
	cairo_translate(cr, origin->x + pit->second.x, origin->y + pit->second.y);
	cairo_arc(cr, 0, 0, 3, 0, 2 * M_PI);
	cairo_fill(cr);
}

static void do_drawing(cairo_t *cr, GtkWidget* widget)
{
	cout << "do_drawing" << endl;
	static int tmp_time = 0;
	
	GtkWidget *win = gtk_widget_get_toplevel(widget);
  
	int width, height;
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
  
	
	Vector origin(width/2, height/2, 0);

	cairo_set_line_width(cr, 3); 
	cairo_set_source_rgb(cr, 0 ,0, 0.69);
	cairo_translate(cr, origin.x, origin.y);
	cairo_arc(cr, 0, 0, 3, 0, 2 * M_PI);
	cairo_fill(cr);

	for (vector<Molecule>::iterator it = molecules.begin(); it != molecules.end(); it++) {
		cairo_identity_matrix(cr);
		cairo_translate(cr, origin.x, origin.y);

		cairo_set_line_width(cr, 1);  
		cairo_set_source_rgb(cr, 0 , 0.69, 0);

		std::map<long, Vector> h = it->histogram();
		std::map<long, Vector>::iterator phit = h.begin();
		std::map<long, Vector>::iterator hit = ++h.begin();
		for ( ; hit != h.end(); hit++, phit++)
		{
			cairo_move_to(cr, phit->second.x , phit->second.y);
			cairo_line_to(cr, hit->second.x , hit->second.y);
			cairo_stroke(cr);
		}		
	}

	for (vector<Molecule>::iterator it = molecules.begin(); it != molecules.end(); it++) {
		// cairo_set_line_width(cr, 3);
		do_drawing_molecule_at(cr, &(*it), &origin, tmp_time);
	}
	
	tmp_time +=5;
}


void simulate()
{
	cout << "Starting simulation" << endl;
	BrownianMotion bm;
	Vector p(0, 0, 0);
	for (int i = 0; i < NUMBER_OF_MOLECULES; i++)
	{
		molecules.push_back(Molecule(p, 1, CairoColor(0.69, 0.19, 0)));
	}
	

	long sim_time = 0;
	for (int i = 0; i < NUMBER_OF_ITERATIONS; i++)
	{
		sim_time += 5;
		for (vector<Molecule>::iterator it = molecules.begin(); it != molecules.end(); ++it) {
			it->move(sim_time, bm.get_move());				
		}
	}
	cout << "Simulation finished." << endl;
}

static gpointer
thread_func( gpointer data )
{
	simulate();
	return NULL;
}

static float diffusion_coefficient(float temperature_K, float viscosity_eta, float diameter)
{
	static float k_b = 1.38E-10;
	
	return k_b * temperature_K / ( 3 * M_PI * diameter * viscosity_eta);
}

static float squared_displacement(int dimensions, float diffusion_coefficient, float tau )
{
	return 2 * dimensions * diffusion_coefficient * tau;
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
	GThread   *thread;
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
	gtk_window_set_default_size(GTK_WINDOW(window), 600, 600); 
	gtk_window_set_title(GTK_WINDOW(window), "Sim");

	g_timeout_add(3*300, (GSourceFunc) time_handler, (gpointer) window);


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

