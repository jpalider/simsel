#include <string>

#include "GNUPlotter.h"

int main(int argc, char** argv)
{
	std::string s = std::string(argv[1]);
	GNUPlotter gp(s, "using 1:2 every ::0::6");
	gp.set_step(1000);
	gp.set_start(0);
	gp.set_end(5*1000);
	gp.set_xstart(0);
	gp.set_xend(4);
	gp.set_ystart(0);
	gp.set_yend(500*1000);
	gp.plot();
}
