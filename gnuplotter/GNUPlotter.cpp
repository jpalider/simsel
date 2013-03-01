#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <iostream>

#include "GNUPlotter.h"

using namespace std;

GNUPlotter::GNUPlotter(string file_prefix)
{
	gpfile_prefix = file_prefix;
	gpstep = 10*1000;
	gpstart = 0;
	gpend = 5*1000*1000;
	gpxstart = 0;
	gpxend = 500*1000;
	gpystart = 0;
	gpyend = 10*1000;
}

//http://stackoverflow.com/questions/3521209/making-c-code-plot-a-graph-automatically
void GNUPlotter::plot()
{
//	const char * commandsForGnuplot[] = {"set title \"TITLEEEEE\"", "plot 'data.temp'"};
	FILE * gnuplotPipe = popen ("gnuplot -p", "w");

	for (int i = gpstart; i < gpend; i+= gpstep)
	{
		stringstream ss;
		ss << i;
		string ap = "\"";
		fprintf(gnuplotPipe, "%s\n", gpxrange.c_str());
		fprintf(gnuplotPipe, "%s\n", gpyrange.c_str());

		string command =  string("plot ") + ap + gpfile_prefix + ss.str() + string(".txt") + ap + string(" ") + gpplot_option;
		fprintf(gnuplotPipe, "%s\n", command.c_str() );
		fflush(gnuplotPipe);		
	}
}

void GNUPlotter::set_ext(string plot_option)
{
	gpplot_option = plot_option;
}

void GNUPlotter::set_step(int step)
{
	gpstep = step;
}

void GNUPlotter::set_start(int start)
{
	gpstart = start;
}

void GNUPlotter::set_end(int end)
{
	gpend = end;
}

void GNUPlotter::set_xstart(float xstart)
{
	gpxstart = xstart;

	stringstream ss;
	ss << gpxstart << ":" << gpxend;
	gpxrange = string("set xrange [") + ss.str() + string("]");
}

void GNUPlotter::set_xend(float xend)
{
	gpxend = xend;

	stringstream ss;
	ss << gpxstart << ":" << gpxend;
	gpxrange = string("set xrange [") + ss.str() + string("]");
}

void GNUPlotter::set_ystart(float ystart)
{
	gpystart = ystart;

	stringstream ss;
	ss << gpystart << ":" << gpyend;
	gpyrange = string("set yrange [") + ss.str() + string("]");
}

void GNUPlotter::set_yend(float yend)
{
	gpyend = yend;
	stringstream ss;
	ss << gpystart << ":" << gpyend;
	gpyrange = string("set yrange [") + ss.str() + string("]");

}
