/**
 * TODO:
 * ? releasing molecule not a point but a sphere
 * - serialising (protocol buffer?)
 * - test / verification
 * - overlapping cells
 * - intermolecule collision
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>

#ifndef NOGUI
#include <gtk/gtk.h>
#endif
#ifndef NOGUI
#include "Player.h"
#endif

#include "Molecule.h"
#include "Vector.h"
#include "BrownianMotion.h"
#include "Simulation.h"
#include "Statistics.h"
#include "StatisticsDistribution.h"
#include "StatisticsDensity.h"
#include "Boundary.h"
#include "tri_logger/tri_logger.hpp"
#include "Test.h"

using namespace std;


Simulation* s;

#ifndef NOGUI
Player* p;
long vistime = 0;

void do_drawing(cairo_t *, GtkWidget* widget);

gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
    gpointer user_data)
{      
  cr = gdk_cairo_create(gtk_widget_get_window(widget));
  p->do_drawing(cr, widget);
  cairo_destroy(cr);

  return FALSE;
}


gboolean
time_handler(GtkWidget *widget)
{
	//cout << vistime << " " << s->time() << endl;
	if (!s->started())
	{
		return true;
	}

	if (vistime + 1 > s->time())
	{
		if (s->finished())
		{
			return false;
		}
		return true;
	}

	vistime++;
	gtk_widget_queue_draw(widget);
	return true;
}

static gpointer thread_func( gpointer data )
{
	s->run();
	return NULL;
}
#endif

static const long INTERVAL_10_US = 10*1000;

static const long INTERVAL_100_US = 100*1000;

int main(int argc, char **argv)
{
	TRI_LOG_STR("Welcome to the Sim");

	if (argc > 1)
	{
		if (string(argv[1]).find("test") != std::string::npos ||
		    string(argv[1]).find("-t")   != std::string::npos)
		{
			test_box_intersection();
			test_box_intersection2();
			test_sphere_containing();
			test_box_containing();
			test_Po();
			test_to();
			test_or();
			TRI_LOG_STR("All tests passed.");
			return 0;
		}
	}

	if (argc > 1)
	{
		std::string config(argv[1]);
		// here may come some test for file existence and permissions
		s = new Simulation(config);
	}
	else
	{
		s = new Simulation();
	}

	Statistics* stat;
	// stat = new StatisticsDistribution(s, INTERVAL_10_US);
	stat = new StatisticsDensity(s, s->interval());
	s->add(stat);
	// p = new Player(s);

	s->run();
	return 0;

	// if (!p->show())
	// {
	// 	s->run();
	// 	return 0;
	// }

	// if( ! g_thread_supported() )
	// 	g_thread_init( NULL );

	// gdk_threads_init();

	// gdk_threads_enter();

	// GtkWidget *window;
	// GtkWidget *darea;
	// GThread *thread;
	// GError *error = NULL;

	// gtk_init(&argc, &argv);

	// window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	// darea = gtk_drawing_area_new();
	// gtk_container_add(GTK_CONTAINER(window), darea);

	// g_signal_connect(G_OBJECT(darea), "draw", 
	// 		 G_CALLBACK(on_draw_event), NULL); 
	// g_signal_connect(window, "destroy",
	// 		 G_CALLBACK(gtk_main_quit), NULL);

	// gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	// gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);
	// gtk_window_set_title(GTK_WINDOW(window), "Sim");

	// g_timeout_add(p->interval_ms(), (GSourceFunc) time_handler, (gpointer) window);

	// // thread = g_thread_create( thread_func, NULL, FALSE, &error );
	// // if( ! thread )
	// // {
	// // 	g_print( "Error: %s\n", error->message );
	// // 	return( -1 );
	// // }

	// gtk_widget_show_all(window);

	// gtk_main();

	// // gdk_threads_leave();

	return 0;
}

