#include "gnuplot.h"
#include <chrono>
#include <thread>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

std::ofstream getFile( const fs::path& path, bool append = false ){
	std::ofstream of;
	of.open( path.c_str(),
		append ? std::ios_base::app : std::ios_base::trunc
	);
	if (of){
		return of;
	}
	throw std::runtime_error(
		"Could not write to file \"" + path.string() + "\"!"
	);
}

void writeLine(
	const std::string& line,
	const fs::path& path,
	bool append = false
){
	std::ofstream of { getFile(path, append) };
	of << line << '\n';
	of.close();
}


class LineWriter {
private:
	fs::path m_path;
	bool m_append {false};
public:
	LineWriter(const fs::path& p) : m_path {p} {}
	void append(bool b){ m_append = b; }

	void write( const std::string& str ){
		writeLine(str, m_path, m_append);
	}
};

int main(){
	const fs::path wd { fs::current_path().lexically_normal() };
	double xmin {0}, xmax {5}, xstep {0.1};
	int sleep_ms {100};

	using Func = double (*)(double);
	Func fn { std::sin };

	std::string outf { "outf.dat" };
	fs::path outp { wd / outf };
	LineWriter lw {outp};
	lw.write("title_x title_y");
	lw.append(true);

	std::stringstream plotCommand; plotCommand
		<< "plot "
		<< '\"' << outf << '\"' // file
		<< " using 1:2"         // cols
		<< " with lines"        // style
		;
	std::stringstream rng; rng
		<< "set xrange [" << xmin << ':' << xmax << ']';

	GnuplotPipe gp;
	gp.open();
	gp.sendLine( "cd \"" + wd.string() + "\"" );
	gp.sendLine( "set key autotitle columnhead" );
	gp.sendLine( rng.str() );

	bool first {true};
	for ( double x{xmin}; x<xmax; x+=xstep ){
		lw.write( std::to_string(x) + " " + std::to_string( std::sin(x) ) );
		if (first){
			first = false;
			gp.sendLine( plotCommand.str() );
			gp.flush();
		} else {
			gp.sendLine("replot");
			gp.flush();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms));
	}

	return 0;
}
