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
#include <boost/algorithm/string/case_conv.hpp>


#include <config.h>
#include <SimulationControl/simulation_control.h>
#include <UserInterfaces/glut_visualizer.h>
#include <SzenarioGenerator/szenario_generator.h>
#include <SzenarioGenerator/formation_generator.h>
#include <Utilities/console_output.h>

#include "aiee.h"
#include "mubalabieeyes.h"


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

	po::options_description generation_options("Generator options");
	generation_options.add_options()
		("generate", "switch to generator mode")
		("distr-pos-circle", po::value<double>(), "circle radius")
		("distr-pos-circle-angle", po::value<double>()->default_value(15.0), "density of robots")
		("seed", po::value<unsigned int>()->default_value(1), "seed for random number generator")
		("robots", po::value<unsigned int>()->default_value(100), "number of robots")
		("algorithm", po::value<std::string>()->default_value("NONE"), "name of algorithm or lua-file")
		("swarmfile", po::value<std::string>()->default_value("newrandom"), "swarm-file for output")
		("robotfile", "robot-file for output")
		("obstaclefile", "obstacle-file for output")
		("add-pos-handler", "add position request handler for testing")
		("add-vel-handler", "add velocity request handler for testing")
		("add-acc-handler", "add acceleration request handler for testing")
		("distr-pos", po::value<double>()->default_value(0), "distribute position in cube [0;distr-pos]^3")
		("distr-gauss-pos", po::value<double>(), "bounding box for gaussian distribution [0;distr-pos]^3")
		("mean-pos", po::value<double>(), "mean for gaussian distribution (positions)")
		("sigma-pos", po::value<double>(), "sigma for gaussian distribution (position)")
		("mean-vel", po::value<double>(), "mean for gaussian distribution (velocities)")
		("sigma-vel", po::value<double>(), "sigma for gaussian distribution (velocities)")
		("mean-acc", po::value<double>(), "mean for gaussian distribution (accelerations)")
		("sigma-acc", po::value<double>(), "sigma for gaussian distribution (accelerations)")
		("random-walk", "random walk formation generator is used")
		("rnd-walk-steps", po::value<unsigned int>()->default_value(100), "numer of random steps")
		("rnd-min-distance", po::value<double>()->default_value(100.0), "minimum distance of a random step")
		("rnd-max-distance", po::value<double>()->default_value(2500.0), "maximum distance of a random step")
		("cluster", "cluster formation generator is used")
		("clt-clusters", po::value<unsigned int>()->default_value(3), "number of clusters")
		("clt-min-distance", po::value<double>()->default_value(1.0), "minimum distance of a cluster center from the origin")
		("clt-max-distance", po::value<double>()->default_value(25.0), "maximum distance of a cluster center from the origin")
		("clt-sigma", po::value<double>()->default_value(1.0), "sigma for gaussian distribution around the cluster centres")
		("poisson-disc", "poisson disc formation generator is used")
		("min-vel", po::value<double>()->default_value(0), "distribute velocity in sphere with minimal absolute value min-vel")
		("max-vel", po::value<double>()->default_value(0), "distribute velocity in sphere with maximal absolute value max-vel")
		("min-acc", po::value<double>()->default_value(0), "distribute acceleration in sphere with minimal absolute value min-acc")
		("max-acc", po::value<double>()->default_value(0), "distribute acceleration in sphere with maximal absolute value max-acc")
		("cosys-rotate-x", "rotate x-axis of robot coordinate-systems uniformly")
		("cosys-rotate-y", "rotate y-axis of robot coordinate-systems uniformly")
		("cosys-rotate-z", "rotate z-axis of robot coordinate-systems uniformly")
		("cosys-scale-x", "scale x-axis-vector of robot coordinate-systems uniformly in range (0,1]")
		("cosys-scale-y", "scale y-axis-vector of robot coordinate-systems uniformly in range (0,1]")
		("cosys-scale-z", "scale z-axis-vector of robot coordinate-systems uniformly in range (0,1]");

	po::options_description simulation_options("Simulation options");
	simulation_options.add_options()
		("project-file", po::value<std::string>(), "Project file to load")
		("output", po::value<std::string>()->default_value(""), "Path to directory for output")
		("history-length", po::value<unsigned int>()->default_value(25), "history length")
		("dry", "disables statistic output")
		("steps", po::value<unsigned int>(), "number of steps for blind mode")
		("blind", "disables visualization");

	// hidden option list, pssst ;-)
	po::options_description top_secret_options("Top Secret");
	top_secret_options.add_options()("aiee", "");
	top_secret_options.add_options()("mubalabieeyes", "");

	po::options_description options;
	options.add(general_options).add(generation_options).add(simulation_options).add(top_secret_options);

	po::options_description options_help_list;
	options_help_list.add(general_options).add(generation_options).add(simulation_options);

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
	if (vm.count("mubalabieeyes")) {
		mubalabieeyes();
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

	/*
	 * Generate a new szenario
	 */
	if (vm.count("generate")) {
		try {
			// init
			ScenarioGenerator generator(vm["seed"].as<unsigned int>());	// set seed
			generator.init(vm);				// init distribution generator

			// files
			generator.set_worldFile(vm["swarmfile"].as<std::string>());
			if (vm.count("robotfile"))
				generator.set_robotFile(vm["robotfile"].as<std::string>());
			else
				generator.set_robotFile(vm["swarmfile"].as<std::string>());
			if (vm.count("obstaclefile"))
				generator.set_obstacleFile(vm["obstaclefile"].as<std::string>());
			else
				generator.set_obstacleFile(vm["swarmfile"].as<std::string>());

			// distribute everything
			generator.distribute();

			// always start coord-system distributer
			generator.distribute_coordsys(vm);

			// sets request handler if requested
			if (vm.count("add-pos-handler"))
				generator.add_play_pos_request_handler();
			if (vm.count("add-vel-handler"))
				generator.add_play_vel_request_handler();
			if (vm.count("add-acc-handler"))
				generator.add_play_acc_request_handler();

			// write to file
			generator.write_to_file();

			ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Robots were generated!";
			ConsoleOutput::log(ConsoleOutput::Statistics, ConsoleOutput::info) << "Please see file: "
			                                                                   << vm["swarmfile"].as<std::string>()
			                                                                   << ".swarm";
		}
		catch (std::exception& e) {
			ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << e.what();
			throw;
		}
		catch(...) {
			ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "Uncaught unknown exception.";
			throw; //rethrow exception
		}

		return 1;
	}

	// make sure we got a project file as input
	if (!vm.count("project-file")) {
		std::cout << "Usage: '" << argv[0] << " --project-file <filename>'" << std::endl;
		std::cout << "   or: '" << argv[0] << " --help' for additional options" << std::endl;
		return 1;
	}

	try {
		std::string tmpProjectFile = vm["project-file"].as<std::string>();

		// deletes ".swarm" from end of file, if used
		if (tmpProjectFile.rfind(".swarm")!=std::string::npos)
			tmpProjectFile.erase (tmpProjectFile.rfind(".swarm"),6);


		// checks iff statistics shall be created
		bool create_statistics = !vm.count("dry");

		// checks iff visualization should be created and checks if steps is set
		bool create_visualization = !vm.count("blind");
		if(!create_visualization) {
			std::cout << "running in blind mode" << std::endl;

			if(!vm.count("steps")) {
				std::cout << "If --blind is set, --steps need to be set to prevent infinite loop" << std::endl;
				return 1;
			}

			if(!create_statistics) {
				std::cout << "Warning: No Statistics and No Visualisation set" << std::endl;
				std::cout << "If a man speaks in the forest and there is no woman there to hear it, is he still wrong?" << std::endl;
			}
		}

		// create simulation kernel
		boost::shared_ptr<SimulationControl> sim_control(new SimulationControl());
		if(!vm.count("steps")) {
			std::cout <<  "creating simulation without limited steps" << std::endl;
			sim_control->create_new_simulation(tmpProjectFile,
					                           vm["history-length"].as<unsigned int>(),
					                           vm["output"].as<std::string>(),
					                           create_statistics,
					                           false,
					                           0);
		} else {
			std::cout <<  "creating simulation with limited steps" << std::endl;
			sim_control->create_new_simulation(tmpProjectFile,
			                                   vm["history-length"].as<unsigned int>(),
			                                   vm["output"].as<std::string>(),
			                                   create_statistics,
			                                   true,
			                                   vm["steps"].as<unsigned int>());
		}

		boost::shared_ptr<GlutVisualizer> visualizer;
		if(create_visualization) {
			visualizer.reset(new GlutVisualizer(*sim_control));
			visualizer->init();
			sim_control->set_visualizer(visualizer);
			Vector3d cam_pos = string_to_vec( sim_control->camera_position() );
			Vector3d cam_dir = string_to_vec( sim_control->camera_direction() );
			visualizer->set_free_cam_para( cam_pos, cam_dir );

			sim_control->start_simulation();

			// enter visualization's main-loop
			visualizer->glutMainLoop();
		} else {
			// start the simulation thread
			sim_control->start_simulation();
			// simulations should be consumed as fast as possible
			sim_control->set_processing_time_delta(10000);
			unsigned int history_length = vm["history-length"].as<unsigned int>();
			while(!sim_control->is_simulation_finished()) {
				// clean out history
				for(int i = 0; i < history_length; i++) {
					sim_control->process_simulation();
				}
				// wait for the simulation thread to refill history
				boost::xtime xt;
				boost::xtime_get(&xt, boost::TIME_UTC);
				xt.sec += 1; // change xt to next second
				boost::thread::sleep(xt);
			}
			ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::info) << "Terminating simulation..";
			sim_control->terminate_simulation();
			ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::info) << "Simulation finished. Have a good day.";
		}

	}
	catch(std::exception& e) {
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << e.what();
		throw; //rethrow exception
	}
	catch(...) {
		ConsoleOutput::log(ConsoleOutput::Kernel, ConsoleOutput::error) << "Uncaught unknown exception.";
		throw; //rethrow exception
	}
	return 0;
}
