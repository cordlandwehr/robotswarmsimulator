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

#include "stats_out.h"

int StatsOut::stat_output_width = 20;
char StatsOut::stat_gnuplot_date [80];
char StatsOut::stat_output_date [80];

StatsOut::StatsOut() {
}

StatsOut::~StatsOut() {
}

void StatsOut::create_date() {
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
};

void StatsOut::set_id(std::string stat_id) {
	if (this->stat_id.length() != 0) {
		std::cerr << "stat_id already set to " << this->stat_id << std::endl;
	} else {
		this->stat_id = stat_id;
	}
};

void StatsOut::set_dir(std::string stat_dir) {
	if (this->stat_dir.length() != 0) {
		std::cerr << "stat_dir already set to " << this->stat_dir << std::endl;
	} else {
		this->stat_dir = stat_dir;
	}
};


void StatsOut::open(std::vector<std::string> stat_designation) {
//	char Stat_Time [80];
	int j;
	char* stat_temp;

//	time_t Stat_Rawtime;
//	struct tm * Stat_Timeinfo;

//	time ( &Stat_Rawtime );
//	Stat_Timeinfo = localtime ( &Stat_Rawtime );

//	strftime (Stat_Time,80,"%Y_%M%d_%H%M%S",Stat_Timeinfo);
		//year (four digits), month, (day of the month), Hour (in 24h format), Minute, Second
		// http://www.cplusplus.com/reference/clibrary/ctime/strftime.html



	stat_temp = const_cast<char*>(stat_id.c_str());
/*
	std::strcpy (Stat_Gnuplot_filename, "gnuplot_");
	std::strcat (Stat_Gnuplot_filename, Stat_Time);
	std::strcat (Stat_Gnuplot_filename,"_");
	std::strcat (Stat_Gnuplot_filename,Stat_Temp);
	std::strcat (Stat_Gnuplot_filename,".plt");

	std::strcpy (Stat_Output_filname, "Output_");
	std::strcat (Stat_Output_filname, Stat_Time);
	std::strcat (Stat_Output_filname,"_");
	std::strcat (Stat_Output_filname,Stat_Temp);
	std::strcat (Stat_Output_filname,".plt");
*/

	std::strcpy (stat_gnuplot_filename, stat_dir.c_str());
	std::strcat (stat_gnuplot_filename, stat_gnuplot_date);
	std::strcat (stat_gnuplot_filename,"_");
	std::strcat (stat_gnuplot_filename,stat_temp);
	std::strcat (stat_gnuplot_filename,".plt");

	std::strcpy (stat_output_filename, stat_dir.c_str());
	std::strcat (stat_output_filename, stat_output_date);
	std::strcat (stat_output_filename,"_");
	std::strcat (stat_output_filename,stat_temp);
	std::strcat (stat_output_filename,".plt");

	/*
	 *  Building of the Gnuplot-Configuration-File
	 */
	std::ofstream stat_gnuplotfile (stat_gnuplot_filename);

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

	for (j=1; j < (int) stat_designation.size(); j++) {
		stat_gnuplotfile << " \"" << stat_output_filename << "\" using 1:"
						 << j+1
						 << " title \""
						 << stat_designation[j-1]
						 << "\" with linespoints,";
	}

	stat_gnuplotfile << " \"" << stat_output_filename << "\" using 1:"
					<< stat_designation.size()+1
					<< " title \""
					<< stat_designation[stat_designation.size()-1]
					<< "\" with linespoints"
					<< std::endl;

	stat_gnuplotfile.close();

	/*
	 *  Building the skeletal structure of the output-file
	 */

	stat_output.open(stat_output_filename, std::ios::app);

	stat_output << std::right
				<< std::setw(stat_output_width)
				<< "# Time  ";

	for (j=0; j < (int) stat_designation.size(); j++) {
		stat_output << std::setw(stat_output_width)
					<< stat_designation[j] << "  ";
	}

	stat_output << std::endl;
};


void StatsOut::update(int stat_timestep, std::vector<double> stat_data) {
	int j;

	stat_output << std::right
				<< std::setw(stat_output_width) << stat_timestep
				<< "  ";

	for (j=0; j < (int) stat_data.size(); j++) {
		stat_output << std::setw(stat_output_width) << stat_data[j] << "  ";
	}

	stat_output << std::endl;
};


void StatsOut::quit() {

	stat_output << "# simulation quit" << std::endl;

	stat_output.close();
};
