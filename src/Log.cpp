#include "pch.h"
#include "Log.h"

using namespace Log;

namespace pt = boost::posix_time;

static std::string levelName[Logger::NUM_LEVELS] = {
	"[Crt]",
	"[Err]",
	"[Wrn]",
	"[Msg]",
	"[Dbg]",
	"[Trc]",
};

Logger::Logger()
: out("log.txt"), loglvl(Msg)
{
	pt::time_facet *facet = new pt::time_facet("%H:%M:%S");
	out.imbue(std::locale(out.getloc(), facet));
}

std::ostream& Logger::GetStream(int lvl)
{
	if (lvl > loglvl)
		return std::cerr;

	pt::ptime now = pt::second_clock::local_time();
	out << now << ' ' << levelName[lvl] <<  ' ';
	return out;
}

void Logger::RedirectStdStreams()
{
	fcerr.open("cerr.txt");
	if (!fcerr)
		GetStream(Error) << "Failed to redirect cerr, cannot open cerr.txt" << "\n";
	else
		std::cerr.rdbuf(fcerr.rdbuf());

	fcout.open("cout.txt");
	if (!fcout)
		GetStream(Error) << "Failed to redirect cout, cannot open cout.txt" << "\n";
	else
		std::cout.rdbuf(fcout.rdbuf());
}

