/**
 * \class	StatsOut
 * \author	Christoph Weddemann and Sven Kurras
 * \brief	logs statistical data to a file
 *
 * For logging the results of the calculations made in StatsCalc
 * one instance of StatsOut for each subset is created.
 * For each such subset then two files will be generated:
 * 1. gnuplot_<timestamp>_<subsetid>.plt
 * 2. output_<timestamp>_<subsetid>.plt
 * While the gnuplot-file contains header-information for latter
 * displaying the statistical data with GNUPlot, the output-file
 * simply writes one row for a vector of values, each seperated by at
 * least one space. To give these 'columns' a name one might pass
 * an information-vector of same length to the open(...)-function.
 * The names are written as a comment-line at the beginning of the file.
 * For more information on their file-content see the user-guide.
 *
 * Additionally it is possible to use StatsOut more general and
 * drop the gnuplot-file by calling open(...) with parameter
 * gnuPlot=false. This is done for logging the datadump.
 *
 * All StatsOut-files of a simulation are sharing the same timestamp.
 *
 * usage-example:
 * 1. Create new instance(s) and set an unique ID for them (e.g. the subset-name).
 * 2. Call static StatsOut::create_date() for creating a global timestamp.
 * 3. Call open(...) for each instance and pass the column-titles.
 * 4. Repeatedly call update(...) for logging the passed values.
 * 5. Call quit().
 */

#ifndef STATS_OUT_H_
#define STATS_OUT_H_

#include <string>
#include <vector>
#include <fstream>

class StatsOut {
public:
	/**
	 * creates a new instance without any Id.
	 * The id must be set by calling set_id(...)
	 * before the first time calling open(...)
	 */
	StatsOut();

	/**
	 * creates a new instance for the given Id.
	 * Must be unique under all instances opened with
	 * the same timestamp.
	 */
	StatsOut(std::string stat_id);

	/**
	 * creates a new instance for the given Id.
 	 * Additionally sets the basedirectory for the logfile.
	 */
	StatsOut(std::string stat_id, std::string stat_dir);

	virtual ~StatsOut();

	/**
	 * width of each column in output-file
	 */
	static int stat_output_width;

	/**
	 * absolute filename of output-log-file (incl. date and stat-id)
	 */
	static char stat_output_date [80];
	static char stat_gnuplot_date [80];

	/**
	 * absolute filename of gnuplot-config-file (incl. date and stat-id)
	 */
	char stat_gnuplot_filename [80];

	/**
	 * absolute filename of output-log-file (incl. date and stat-id)
	 */
	char stat_output_filename [80];

	/**
	 * filestream to output-log-file
	 */
	std::ofstream stat_output;

	/**
	 * the id - must be unique under all instances with the same timestamp
	 */
	std::string stat_id;

	/**
	 * the directory for the outputfiles incl. trailing pathseperator
	 */
	std::string stat_dir;

	/**
	 * true between the calls of open(...) and quit()
	 */
	bool is_open_;

	/**
	 * creates a new timestamp that is used by all latter calls
	 * to open(...) of any instance (!) for creating the filenames.
	 */
	static void create_date();

	/**
	 * sets the id for this instance that is used by a latter call
	 * to open(...) for creating the filenames.
	 */
	void set_id(std::string stat_id);

	/**
	 * sets the directory for this instance that is used by a latter call
	 * to open(...) for creating the filenames.
	 */
	void set_dir(std::string stat_dir);

	/**
	 * Open this instance with the given column-titles.
	 * Creates the output-file (and the gnuplot-file iff gnuPlot==true) and
	 * opens a stream to it.
	 */
	void open(std::vector<std::string> stat_designation, bool gnuPlot=true);

	/**
	 * logs the given information to the filestream
	 */
	void update(int stat_timestep, std::vector<double> stat_data);

	/**
	 * whether or not this instance is between the calls of open(...) and quit()
	 */
	const bool is_open() const;

	/**
	 * quits this instance and closes all filestreams.
	 */
	void quit();
};

#endif /* STATS_OUT_H_ */
