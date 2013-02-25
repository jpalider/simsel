#ifndef SIM_GNUPLOTTER_H
#define SIM_GNUPLOTTER_H

#include <string>

class GNUPlotter
{
private:
	std::string gpfile_prefix;
	std::string gpplot_option;
	int gpstep;
	float gpstart;
	float gpend;
	float gpxstart;
	float gpxend;
	float gpystart;
	float gpyend;

	std::string gpxrange;
	std::string gpyrange;
public:
	GNUPlotter(std::string file_prefix, std::string plot_option);

	void set_step(int step);
	void set_start(float start);
	void set_end(float end);
	void set_xstart(float xstart);
	void set_xend(float xend);
	void set_ystart(float ystart);
	void set_yend(float yend);
	
	void plot();
};

#endif /* SIM_GNUPLOTTER_H */
