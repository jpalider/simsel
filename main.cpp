#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

#include <cairo.h>
#include <gtk/gtk.h>

#include "Molecule.h"
#include "Position.h"
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

// static void do_drawing_molecule(cairo_t *cr, Molecule* m, Position* origin)
// {
// 	cairo_identity_matrix(cr);
// 	cairo_translate(cr, origin->x + m->get_position().x, origin->y + m->get_position().y);
// 	cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
// 	cairo_fill(cr);
// }

static void do_drawing_molecule_at(cairo_t *cr, Molecule* m, Position* origin, long t)
{
//	Position p = m->get_histogram().at(t);
	map<long, Position>h = m->get_histogram();
	map<long, Position>::iterator pit = h.find(t);
	if (pit == h.end())
		return;
	cairo_identity_matrix(cr);
	cairo_translate(cr, origin->x + pit->second.x, origin->y + pit->second.y);
	cairo_arc(cr, 0, 0, 1, 0, 2 * M_PI);
	cairo_fill(cr);
}

static void do_drawing(cairo_t *cr, GtkWidget* widget)
{
	cout << "do_drawing" << endl;
	static int tmp_time = 0;
	
	GtkWidget *win = gtk_widget_get_toplevel(widget);
  
	int width, height;
	gtk_window_get_size(GTK_WINDOW(win), &width, &height);
  
	
	Position origin(width/2, height/2, 0);

	cairo_set_line_width(cr, 3);  
	cairo_set_source_rgb(cr, 0 ,0, 0.69);
	cairo_translate(cr, origin.x, origin.y);
	cairo_arc(cr, 0, 0, 3, 0, 2 * M_PI);
	cairo_fill(cr);

	for (vector<Molecule>::iterator it = molecules.begin(); it != molecules.end(); it++) {

		cairo_set_line_width(cr, 1);  
		cairo_set_source_rgb(cr, 0 , 0.69, 0);

		std::map<long, Position> h = it->get_histogram();
		std::map<long, Position>::iterator phit = h.begin();
		std::map<long, Position>::iterator hit = ++h.begin();
		for ( ; hit != h.end(); hit++, phit++)
		{
			cairo_move_to(cr, phit->second.x , phit->second.y);
			cairo_line_to(cr, hit->second.x , hit->second.y);
			cairo_stroke(cr);
		}
		cairo_set_line_width(cr, 2);
		cairo_set_source_rgb(cr, 0.69, 0.19, 0);
		do_drawing_molecule_at(cr, &(*it), &origin, tmp_time);
	}
	
	tmp_time +=5;
}

static float squared_distance_between_points(Position p1, Position p2)
{
	return (p2.x-p1.x)*(p2.x-p1.x) + (p2.y-p1.y)*(p2.y-p1.y) + (p2.z-p1.z)*(p2.z-p1.z);
}

static float cos_at_ab(float a2, float b2, float c2, float ab)
{
	return (a2 + b2 - c2) / (2 * ab);
}

// Works for 0-180
static bool acute_angle(float cos)
{
	return cos > 0;
}

Position cross_product(Position a, Position b, Position c)
{
	// here Sosition should be replaced by Vector
	Position v(a.x - c.x, a.y - c.y, a.z - c.z);
	Position w(b.x - c.x, b.y - c.y, b.z - c.z);
		
	return Position(v.y*w.z - w.y*v.z, w.x*v.z - v.x*w.z, v.x*w.y - w.x*v.y);
}

// http://kb.komires.net/article.php?id=2
// http://pl.wikipedia.org/wiki/Iloczyn_wektorowy
// http://www.matematyka.pl/47774.htm
static float triangle_area(Position a, Position b, Position c)
{
	Position axb = cross_product(a, b, c);
	//cout << a << b << c << axb;
	float area = 0.5 * sqrt( squared_distance_between_points(Position(0,0,0), axb) );
	return area;		
}

// http://stackoverflow.com/questions/2062286/testing-whether-a-line-segment-intersects-a-sphere
// -> http://www.matematyka.pl/250033.htm
static bool segment_line_sphere_intersect(Position p1, Position p2, Position s, float r)
{

	float sq_p1_to_s = squared_distance_between_points(p1, s);
	float sq_p2_to_s = squared_distance_between_points(p2, s);
	float sq_p1_to_p2 = squared_distance_between_points(p1, p2);
	float p1_to_p2 = sqrt( sq_p1_to_p2 );
	float p1_to_s = sqrt( sq_p1_to_s );
	float p2_to_s = sqrt( sq_p2_to_s );


	float cos_p1 = cos_at_ab(sq_p1_to_s, sq_p1_to_p2, sq_p2_to_s, p1_to_p2 * p1_to_s );
	float cos_p2 = cos_at_ab(sq_p2_to_s, sq_p1_to_p2, sq_p1_to_s, p1_to_p2 * p2_to_s );

	cout << cos_p1 << " " << acos(cos_p1) * 360 / 2 / M_PI << " " << acute_angle(cos_p1) << endl;
	cout << cos_p2 << " " << acos(cos_p2) * 360 / 2 / M_PI << " " << acute_angle(cos_p2) << endl;

	// maybe move this before 
	if ( acute_angle(cos_p1) )
	{
		if (sq_p1_to_s < r*r)
			return true;
	}

	if ( acute_angle(cos_p2) )
	{
		if (sq_p2_to_s < r*r)
			return true;
	}
	
	if ( acute_angle(cos_p1) && acute_angle(cos_p2) )
	{
		// procedure for intersection
		if ( 2 * triangle_area(p1, p2, s) / p1_to_p2 < r)
			return true;
	}
	
	return false;
	
	
}

void simulate()
{
	cout << "Starting simulation" << endl;
	BrownianMotion bm;
	Position p(0, 0, 0);
	for (int i = 0; i < NUMBER_OF_MOLECULES; i++)
	{
		molecules.push_back(Molecule(p, 1));
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

