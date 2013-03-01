#include <cstdlib>
#include <string>
#include <iostream>

#include "GNUPlotter.h"
#include "optionparser.h"

// struct Arg: public option::Arg
// {
//   static void printError(const char* msg1, const option::Option& opt, const char* msg2)
//   {
//     fprintf(stderr, "%s", msg1);
//     fwrite(opt.name, opt.namelen, 1, stderr);
//     fprintf(stderr, "%s", msg2);
//   }

//   static option::ArgStatus Unknown(const option::Option& option, bool msg)
//   {
//     if (msg) printError("Unknown option '", option, "'\n");
//     return option::ARG_ILLEGAL;
//   }

//   static option::ArgStatus Required(const option::Option& option, bool msg)
//   {
//     if (option.arg != 0)
//       return option::ARG_OK;

//     if (msg) printError("Option '", option, "' requires an argument\n");
//     return option::ARG_ILLEGAL;
//   }

//   static option::ArgStatus NonEmpty(const option::Option& option, bool msg)
//   {
//     if (option.arg != 0 && option.arg[0] != 0)
//       return option::ARG_OK;

//     if (msg) printError("Option '", option, "' requires a non-empty argument\n");
//     return option::ARG_ILLEGAL;
//   }

//   static option::ArgStatus Numeric(const option::Option& option, bool msg)
//   {
//     char* endptr = 0;
//     if (option.arg != 0 && strtol(option.arg, &endptr, 10)){};
//     if (endptr != option.arg && *endptr == 0)
//       return option::ARG_OK;

//     if (msg) printError("Option '", option, "' requires a numeric argument\n");
//     return option::ARG_ILLEGAL;
//   }
// };


enum  optionIndex { UNKNOWN,
		    START,
		    END,
		    STEP,
		    XSTART,
		    XEND,
		    YSTART,
		    YEND,
		    EXT,
		    USING,
		    EVERY,
		    HELP
};

const option::Descriptor usage[] =
{
	{ UNKNOWN, 0,"" , ""    ,option::Arg::Optional,
	 "USAGE: gnuplotter [options] filename_prefix\n\n"
	 "Options:" },
	{ START,    0,"" , "start", option::Arg::Optional, "  --start  \tStart time of plotting." },
	{ END,      0,"" , "end",   option::Arg::Optional, "  --end  \tEnd time of plotting." },
	{ STEP,     0,"" , "step",  option::Arg::Optional, "  --step  \tPlotting step." },
	{ XSTART,   0,"" , "xstart",option::Arg::Optional, "  --xstart  \tStart of x axis." },
	{ XEND,     0,"" , "xend",  option::Arg::Optional, "  --xend  \tEnd of x axis." },
	{ YSTART,   0,"" , "ystart",option::Arg::Optional, "  --ystart  \tStart of y axis." },
	{ YEND,     0,"" , "yend",  option::Arg::Optional, "  --yend  \tEnd of y axis." },
	{ EXT,      0,"" , "ext",   option::Arg::Optional, "  --ext  \tExtension config parameters for gnuplot." },
	{ USING,    0,"" , "using", option::Arg::Optional, "  --using  \tUsing parameter for gnuplot." },
	{ EVERY,    0,"" , "every", option::Arg::Optional, "  --every  \tEvery parameter for gnuplot." },
	{ HELP,     0,"" , "help",  option::Arg::Optional, "  --help  \tPrint usage and exit." },
	{ 0,0,0,0,0,0}
};

using namespace::std;

// gnuplotter -ext "using 1:2 every ::0::6000" filename
int main(int argc, char** argv)
{
	argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
	option::Stats  stats(usage, argc, argv);
	option::Option options[stats.options_max], buffer[stats.buffer_max];
	option::Parser parse(usage, argc, argv, options, buffer);

	if (parse.error())
	{
		cout << "Parsing error" << endl;
		return 1;
	}

	if (options[HELP] || argc == 0) {
		option::printUsage(std::cout, usage);
		return 0;
	}

	for (option::Option* opt = options[UNKNOWN]; opt; opt = opt->next())
		std::cout << "Unknown option: " << opt->name << endl;

	GNUPlotter gp( parse.nonOptionsCount() ? parse.nonOption(0) : "" );

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
	
	if ( options[YSTART] )
		gp.set_ystart(atoi(options[YSTART].arg));
	
	if ( options[YEND] )
		gp.set_yend(atoi(options[YEND].arg));

	string s(" ");
	if ( options[USING] )
		s += string(" using ") + string(options[USING].arg);
	if ( options[EVERY] )
		s += string(" every ") + string(options[EVERY].arg);
	if ( options[EXT] )
		s += string(" ") + string(options[EXT].arg);
	gp.set_ext(s);

	gp.plot();
}
