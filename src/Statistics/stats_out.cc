/*
 * stats_out.cpp
 *
 *  Created on: 02.02.2009
 *      Author: sven
 */

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <iomanip>
#include <ctime>

#include <boost/filesystem/fstream.hpp>
#include <boost/program_options.hpp>

#include "stats_out.h"
#include <Utilities/console_output.h>

int StatsOut::stat_output_width = 20;
char StatsOut::stat_gnuplot_date [80];
char StatsOut::stat_output_date [80];

StatsOut::StatsOut() {
	is_open_ = false;
}

StatsOut::StatsOut(std::string stat_id) {
	this->stat_id = stat_id;
	is_open_ = false;
}

StatsOut::StatsOut(std::string stat_id, std::string stat_dir) {
	this->stat_id = stat_id;
	this->stat_dir_ = boost::filesystem::path(stat_dir);
	is_open_ = false;
}

StatsOut::~StatsOut() {
	if (is_open()) {
		// try to make a clean quit, though this case *should* not occur
		quit();
	}
}

void StatsOut::create_prefixes(std::string s) {
	if (s.length()==0) {
		char stat_time [80];
		time_t stat_rawtime;
		struct tm * stat_timeinfo;

		time ( &stat_rawtime );
		stat_timeinfo = localtime ( &stat_rawtime );

		strftime (stat_time,80,"%Y%m%d_%H%M%S",stat_timeinfo);
		//year (four digits), month, (day of the month), Hour (in 24h format), Minute, Second
		// http://www.cplusplus.com/reference/clibrary/ctime/strftime.html

		std::strcpy (stat_gnuplot_date, "gnuplot_");
		std::strcat (stat_gnuplot_date, stat_time);

		std::strcpy (stat_output_date, "output_");
		std::strcat (stat_output_date, stat_time);
	} else {
		std::strcpy (stat_gnuplot_date, "gnuplot_");
		std::strcat (stat_gnuplot_date, s.c_str());

		std::strcpy (stat_output_date, "output_");
		std::strcat (stat_output_date, s.c_str());
	}
}

void StatsOut::set_id(std::string stat_id) {
	if (this->stat_id.length() != 0) {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::warning) << "stat_id already set to " << this->stat_id;
	} else {
		this->stat_id = stat_id;
	}
}

void StatsOut::set_dir(std::string stat_dir) {
	stat_dir_ = boost::filesystem::path(stat_dir);
}


void StatsOut::open(std::vector<std::string> stat_designation, bool gnuPlot) {
	char* stat_temp;

	stat_temp = const_cast<char*>(stat_id.c_str());

	if (gnuPlot) {
		std::strcpy (stat_gnuplot_filename, stat_gnuplot_date);
		std::strcat (stat_gnuplot_filename,"_");
		std::strcat (stat_gnuplot_filename,stat_temp);
		std::strcat (stat_gnuplot_filename,".plt");
	}

	std::strcpy (stat_output_filename, stat_output_date);
	std::strcat (stat_output_filename,"_");
	std::strcat (stat_output_filename,stat_temp);
	std::strcat (stat_output_filename,".plt");

	/*
	 *  Building of the Gnuplot-Configuration-File
	 */

	if (gnuPlot) {
		boost::filesystem::ofstream stat_gnuplotfile (stat_dir_ / stat_gnuplot_filename);

		stat_gnuplotfile << "# statistics of the simulation" << std::endl
						 << "#====================================" << std::endl
						 << "set title  \" SCHLAUE SCHWÄRME \\n " //ID Datum
						 << "Modell: ...\" " << std::endl // Modell
						 << "set xrange []" << std::endl
						 << "set yrange []" << std::endl
						 << "set grid" << std::endl
						 << "set pointsize 0.5" << std::endl
						 << "set xlabel 'time'" << std::endl
						 << "set ylabel ''" << std::endl
						 << "plot";

		for (unsigned int j=1; j < stat_designation.size(); j++) {
			stat_gnuplotfile << " \"" << stat_output_filename << "\" using 1:"
							 << j+1
							 << " title \""
							 << stat_designation[j-1]
							 << "\" with linespoints,";
		}

		if (stat_designation.size() > 0) {
			stat_gnuplotfile << " \"" << stat_output_filename << "\" using 1:"
							<< stat_designation.size()+1
							<< " title \""
							<< stat_designation[stat_designation.size()-1]
							<< "\" with linespoints"
							<< std::endl;
		}

		stat_gnuplotfile.close();
	}

	/*
	 *  Building the skeletal structure of the output-file
	 */

	stat_output_.open(stat_dir_ / stat_output_filename);

	stat_output_ << std::right
				<< std::setw(stat_output_width)
				<< "# time  ";

	for (unsigned int j=0; j < stat_designation.size(); j++) {
		stat_output_ << std::setw(stat_output_width)
					<< stat_designation[j] << "  ";
	}

	stat_output_ << std::endl;

	is_open_ = true;
};

void StatsOut::update(int stat_timestep, std::vector<double> stat_data) {
	if (!is_open_) {
		ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::warning) << "Logfile for stats not opened";
		return;
	}

	int j;

	stat_output_ << std::right
				<< std::setw(stat_output_width) << stat_timestep
				<< "  ";

	for (j=0; j < (int) stat_data.size(); j++) {
		stat_output_ << std::setw(stat_output_width) << stat_data[j] << "  ";
	}

	stat_output_ << std::endl;
};

const bool StatsOut::is_open() const {
	return is_open_;
}

void StatsOut::quit() {

	stat_output_ << "# simulation quit" << std::endl;

	stat_output_.close();

	is_open_ = false;
};
