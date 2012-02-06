/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
 * robot_swarm_simulator_cmdline.cc
 *
 *  Created on: 21.02.2009
 *      Author: peter
 */

/**
 * \file robot_swarm_simulator_cmdline.cc
 * \brief Main program used to load a project file, setup the simulation and start it.
 */

#include <string>
#include <cstdio>

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>
#include <boost/cast.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string/case_conv.hpp>

#include <config.h>
#include <SimulationControl/simulation_control.h>
#include <Utilities/console_output.h>
#include <Wrapper/lua_distribution_generator.h>

#ifndef RSS_NO_VISUALIZATION
	#include <QtGui/QApplication>
	#include <Gui/rss_main_window.h>
	#include <Gui/rss_gl_widget.h>
#endif

#include "aiee.h"


int main(int argc, char** argv) {
	namespace po = boost::program_options;

	// declaration of program options
	po::options_description general_options("General options");
	general_options.add_options()
		("help", "shows this help message");
	general_options.add_options()
		("version", "shows version of RobotSwarmSimulator");
	general_options.add_options()
		("about", "tells you who developed this awesome piece of software");
	general_options.add_options()
		("logfile", po::value<std::string>(), "file to log to");
	general_options.add_options()
		("loglevel", po::value<std::string>()->default_value("info"), "log level");

	po::options_description simulation_options("Simulation options");
	simulation_options.add_options()
		("project-file", po::value<std::string>(), "Project file to load")
		("output", po::value<std::string>()->default_value(""), "Path to directory for output")
		("history-length", po::value<unsigned int>()->default_value(25), "history length")
		("dry", "disables statistic output")
		("steps", po::value<unsigned int>(), "number of steps for blind mode")
		("blind", "disables visualization")
		("luaseed", po::value<unsigned int>(), "seed for global distribution generator used in lua.")
		("run_until_no_multiplicity", "quits the simulation when no two robots occupy the same point");

	// hidden option list, pssst ;-)
	po::options_description top_secret_options("Top Secret");
	top_secret_options.add_options()("aiee", "");

	po::options_description options;
	options.add(general_options).add(simulation_options).add(top_secret_options);

	po::options_description options_help_list;
	options_help_list.add(general_options).add(simulation_options);

	// create variable map and parse options from command line
	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, options), vm);
		po::notify(vm);
	}
	catch (std::exception& e) {
		std::cout << "[ERROR] " << e.what() << std::endl;
		throw;
	}
	catch(...) {
		std::cout <<  "[ERROR] Uncaught unknown exception." << std::endl;
		throw; //rethrow exception
	}

	// initalize logging system
	ConsoleOutput::Level log_level = ConsoleOutput::none;
	std::string arg_log_level = vm["loglevel"].as<std::string>();

	boost::to_lower(arg_log_level);
	if(arg_log_level == "debug") {
		log_level = ConsoleOutput::debug;
	} else if(arg_log_level == "info") {
		log_level = ConsoleOutput::info;
	} else if(arg_log_level == "warning") {
		log_level = ConsoleOutput::warning;
	} else if(arg_log_level == "error") {
		log_level = ConsoleOutput::error;
	} else if(arg_log_level == "none") {
		log_level = ConsoleOutput::none;
	} else {
		std::cout << "[WARNING] unkown log level: resorting to INFO" << std::endl;
		log_level = ConsoleOutput::info;
	}


	if(vm.count("logfile")) {
		ConsoleOutput::initalize_logging_system(log_level, true, vm["logfile"].as<std::string>());
	} else {
		ConsoleOutput::initalize_logging_system(log_level, false);
	}

	// ppssssstt
	if (vm.count("aiee")) {
		aiee();
		return 0;
	}

	// check whether to show help message and exit
	if (vm.count("help")) {
		std::cout << options_help_list << std::endl;
		return 1;
	}

	// check whether to show version information of RSS
	if (vm.count("version")) {
		std::cout << "Version: " << cfg::version << std::endl;
		return 1;
	}

	// check whether to show about information of RSS
	if (vm.count("about")) {
		std::cout << cfg::about_ascii << std::endl;
		return 1;
	}

	try {
		if (vm.count("project-file")) {
			std::string tmpProjectFile = vm["project-file"].as<std::string>();

			// deletes ".swarm" from end of file, if used
			if (tmpProjectFile.rfind(".swarm")!=std::string::npos)
				tmpProjectFile.erase (tmpProjectFile.rfind(".swarm"),6);

			bool run_until_no_multiplicity = vm.count("run_until_no_multiplicity");

			// initializes the lua random number generator
			if(vm.count("luaseed")) {
				unsigned int luaseed = vm["luaseed"].as<unsigned int>();
				LuaWrapper::lua_generator_set_seed(luaseed);
			}


			// checks iff visualization should be created and checks if steps is set
			bool create_visualization = !vm.count("blind");
			if(!create_visualization) {
				std::cout << "running in blind mode" << std::endl;

				if(!vm.count("steps")) {
					std::cout << "If --blind is set, --steps need to be set to prevent infinite loop" << std::endl;
					return 1;
				}
			}

			// create simulation kernel
			boost::shared_ptr<SimulationControl> sim_control(new SimulationControl());
			if(!vm.count("steps")) {
				std::cout <<  "creating simulation without limited steps" << std::endl;
			sim_control->create_new_simulation(tmpProjectFile,
												   vm["history-length"].as<unsigned int>(),
												   vm["output"].as<std::string>(),
											   false,
												   0,
												   run_until_no_multiplicity);
			} else {
				std::cout <<  "creating simulation with limited steps" << std::endl;
				sim_control->create_new_simulation(tmpProjectFile,
												   vm["history-length"].as<unsigned int>(),
												   vm["output"].as<std::string>(),
												   true,
												   vm["steps"].as<unsigned int>(),
												   run_until_no_multiplicity);
			}

#ifndef RSS_NO_VISUALIZATION
			if(create_visualization) {

				QApplication app(argc, argv);
				Q_INIT_RESOURCE(qt_resources);
				app.setApplicationName("RobotSwarmSimulator");
				app.setOrganizationName("Uni Paderborn");
				app.setOrganizationDomain("uni-paderborn.de");

				RSSMainWindow main_window;

				main_window.init();
				main_window.rss_gl_widget()->set_simulation_control(sim_control);

				main_window.show();

				return app.exec();

			} else {
#else
			{
#endif
				// start the simulation thread
				sim_control->start_simulation();
				// simulations should be consumed as fast as possible
				sim_control->set_processing_time_delta(10000);
				unsigned int history_length = vm["history-length"].as<unsigned int>();
				while(!sim_control->is_simulation_finished()) {
					// clean out history
					for(unsigned int i = 0; i < history_length; i++) {
						sim_control->process_simulation();
					}
					// wait for the simulation thread to refill history
					boost::xtime xt;
					boost::xtime_get(&xt, boost::TIME_UTC);
					xt.sec += 1; // change xt to next second
					boost::this_thread::sleep(boost::posix_time::milliseconds(5));
				}
				ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::info) << "Terminating simulation.\n";
				sim_control->terminate_simulation();
				ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::info) << "Simulation finished. Have a good day.\n";
			}
		} else {

#ifndef RSS_NO_VISUALIZATION
			QApplication app(argc, argv);
			Q_INIT_RESOURCE(qt_resources);
			app.setApplicationName("RobotSwarmSimulator");
			app.setOrganizationName("Uni Paderborn");
			app.setOrganizationDomain("uni-paderborn.de");

			RSSMainWindow main_window;

			main_window.init();
			main_window.show();

			return app.exec();
#else
			std::cout << "Usage: '" << argv[0] << " --project-file <filename>'" << std::endl;
			std::cout << "   or: '" << argv[0] << " --help' for additional options" << std::endl;
			return 1;
#endif
		}

	}
	catch(std::exception& e) {
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << e.what();
		throw; //rethrow exception
	}
	catch(...) {
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "Uncaught unknown exception.\n";
		throw; //rethrow exception
	}	

	return 0;
}
