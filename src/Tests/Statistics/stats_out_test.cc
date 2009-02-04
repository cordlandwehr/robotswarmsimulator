#include <boost/test/unit_test.hpp>
#include <boost/smart_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include <stdio.h>

#include "../../Statistics/stats_out.h"

BOOST_AUTO_TEST_CASE(stats_out_test) {
	int time = 0;

	std::vector<std::string> stat_designation;
	stat_designation.push_back("focus");
	stat_designation.push_back("main point");
	stat_designation.push_back("average value");
	stat_designation.push_back("4");
	stat_designation.push_back("5");
	stat_designation.push_back("6");

	std::vector<double> stat_data_master;
	stat_data_master.push_back(1.5);
	stat_data_master.push_back(100.0);
	stat_data_master.push_back(20.5);
	stat_data_master.push_back(1.5);
	stat_data_master.push_back(100.0);
	stat_data_master.push_back(20.5);

	std::vector<double> stat_data_slave;
	stat_data_slave.push_back(5.5);
	stat_data_slave.push_back(20.0);
	stat_data_slave.push_back(23.5);
	stat_data_slave.push_back(1.5);
	stat_data_slave.push_back(120.0);
	stat_data_slave.push_back(10.5);

	// create temp-file as praefix
	std::string s((const char *) tmpnam(NULL));

	StatsOut master;
	master.set_id("master");
	master.set_dir(s);

	StatsOut slave;
	slave.set_id("slave");
	slave.set_dir(s);

	StatsOut::create_date();

	master.open(stat_designation);
	slave.open(stat_designation);

	for (time = 0; time <= 1000; time++) {
		master.update(time, stat_data_master);
		slave.update(time, stat_data_slave);
	}

	master.quit();
	slave.quit();

	FILE * f;
	f = fopen(&master.stat_gnuplot_filename[0], "r");
	BOOST_CHECK_EQUAL(f!=0, true);
	fclose(f);
	BOOST_CHECK_EQUAL(remove(&master.stat_gnuplot_filename[0]), 0);

	f = fopen(&master.stat_output_filename[0], "r");
	BOOST_CHECK_EQUAL(f!=0, true);
	fclose(f);
	BOOST_CHECK_EQUAL(remove(&master.stat_output_filename[0]), 0);

	f = fopen(&slave.stat_gnuplot_filename[0], "r");
	BOOST_CHECK_EQUAL(f!=0, true);
	fclose(f);
	BOOST_CHECK_EQUAL(remove(&slave.stat_gnuplot_filename[0]), 0);

	f = fopen(&slave.stat_output_filename[0], "r");
	BOOST_CHECK_EQUAL(f!=0, true);
	fclose(f);
	BOOST_CHECK_EQUAL(remove(&slave.stat_output_filename[0]), 0);
}
