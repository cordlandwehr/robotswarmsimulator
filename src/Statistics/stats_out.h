/**
 * \class	StatsOut
 * \author	Christoph Weddemann and Sven Kurras
 * \brief	logs the statistical data in gnuplot-format
 */

#ifndef STATS_OUT_H_
#define STATS_OUT_H_

#include <string>
#include <vector>
#include <fstream>

class StatsOut {
public:
	StatsOut();
	StatsOut(std::string stat_id);
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

	std::string stat_id;

	/**
	 * the directory for the outputfiles incl. trailing pathseperator
	 */
	std::string stat_dir;

	bool is_open_;

	static void create_date();
	void set_id(std::string stat_id);
	void set_dir(std::string stat_dir);
	void open(std::vector<std::string> stat_designation);
	const bool is_open() const;
	void update(int stat_timestep, std::vector<double> stat_data);
	void quit();
};

#endif /* STATS_OUT_H_ */
