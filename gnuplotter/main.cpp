#include <cstdlib>
#include <string>
#include <iostream>

#include "GNUPlotter.h"
#include "optionparser.h"

enum  optionIndex { UNKNOWN,
		    START,
		    END,
		    STEP,
		    XSTART,
		    XEND,
		    YSTART,
		    YEND,
		    EXT,
		    HELP
};

const option::Descriptor usage[] =
{
	{ UNKNOWN, 0,"" , ""    ,option::Arg::None,
	 "USAGE: gnuplotter [options] filename_prefix\n\n"
	 "Options:" },
	{ START,    0,"" , "start",option::Arg::None,
	 "  --start  \tStart time of plotting." },
	{ END,    0,"" , "end",option::Arg::None,
	 "  --end  \tEnd time of plotting." },
	{ STEP,    0,"" , "step",option::Arg::None,
	 "  --step  \tPlotting step." },
	{ XSTART,    0,"" , "xstart",option::Arg::None,
	 "  --xstart  \tStart of x axis." },
	{ XEND,    0,"" , "xend",option::Arg::None,
	 "  --xend  \tEnd of x axis." },
	{ YSTART,    0,"" , "ystart",option::Arg::None,
	 "  --ystart  \tStart of y axis." },
	{ YEND,    0,"" , "yend",option::Arg::None,
	 "  --yend  \tEnd of y axis." },
	{ EXT,    0,"" , "ext",option::Arg::None,
	 "  --ext  \tExtension config parameters for gnuplot." },
	{ HELP,    0,"" , "help",option::Arg::None,
	 "  --help  \tPrint usage and exit." },

	// { UNKNOWN, 0,"" ,  ""   ,option::Arg::None,
	//  "\nExamples:\n"
	//  "  example --unknown -- --this_is_no_option\n"
	//  "  example -unk --plus -ppp file1 file2\n" },
	{ 0,0,0,0,0,0}
};

// gnuplotter -ext "using 1:2 every ::0::6000" filename
int main(int argc, char** argv)
{
	argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
	option::Stats  stats(usage, argc, argv);
	option::Option options[stats.options_max], buffer[stats.buffer_max];
	option::Parser parse(usage, argc, argv, options, buffer);

	if (parse.error())
		return 1;

	if (options[HELP] || argc == 0) {
		option::printUsage(std::cout, usage);
		return 0;
	}
	
	// std::string s = std::string(argv[1]);
	GNUPlotter gp( parse.nonOptionsCount() ? parse.nonOption(0) : "" );

	for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
		std::cout << "Unknown option: " << opt->name << "\n";
	
	if ( options[STEP] )
		gp.set_step(atol(options[STEP].arg));

	if ( options[START] )
		gp.set_start(atol(options[START].arg));
	
	if ( options[END] )
		gp.set_end(atol(options[END].arg));
	
	if ( options[XSTART] )
		gp.set_xstart(atoi(options[XSTART].arg));
	
	if ( options[XEND] )
		gp.set_xend(atoi(options[XEND].arg));
	
	if ( options[YEND] )
		gp.set_ystart(atoi(options[YSTART].arg));
	
	if ( options[YEND] )
		gp.set_yend(atoi(options[YEND].arg));

	if ( options[EXT] )
		gp.set_ext(options[EXT].arg);

	//gp.plot();
}
