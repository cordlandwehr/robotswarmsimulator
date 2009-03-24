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

#include <config.h>
#include <SimulationControl/simulation_control.h>
#include <UserInterfaces/glut_visualizer.h>
#include <Utilities/szenario_generator.h>
#include <Utilities/console_output.h>

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

	po::options_description generation_options("Generator options");
	generation_options.add_options()
		("generate", "switch to generator mode")
		("seed", po::value<unsigned int>()->default_value(1), "seed for random number generator")
		("robots", po::value<unsigned int>()->default_value(100), "number of robots")
		("algorithm", po::value<std::string>()->default_value("NONE"), "name of algorithm or lua-file")
		("swarmfile", po::value<std::string>()->default_value("newrandom"), "swarm-file for output")
		("robotfile", po::value<std::string>()->default_value("newrandom"), "robot-file for output")
		("obstaclefile", po::value<std::string>()->default_value("newrandom"), "obstacle-file for output")
		("add-pos-handler", "add position request handler for testing")
		("add-vel-handler", "add velocity request handler for testing")
		("add-acc-handler", "add acceleration request handler for testing")
		("distr-pos", po::value<double>()->default_value(0), "distribute position in cube [0;distr-pos]^3")
		("min-vel", po::value<double>()->default_value(0), "distribute velocity in sphere with minimal absolute value min-vel")
		("max-vel", po::value<double>()->default_value(0), "distribute velocity in sphere with maximal absolute value max-vel")
		("min-acc", po::value<double>()->default_value(0), "distribute acceleration in sphere with minimal absolute value min-acc")
		("max-acc", po::value<double>()->default_value(0), "distribute acceleration in sphere with maximal absolute value max-acc")
		("distr-coord", "distribute robot coordinate-systems uniformly");

	po::options_description simulation_options("Simulation options");
	simulation_options.add_options()
		("project-file", po::value<std::string>(), "Project file to load")
		("output", po::value<std::string>()->default_value(""), "Path to directory for output")
		("history-length", po::value<unsigned int>()->default_value(25), "history length")
		("dry", "disables statistic output");

	po::options_description options;
	options.add(general_options).add(generation_options).add(simulation_options);

	// create variable map and parse options from command line
	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, options), vm);
		po::notify(vm);
	}
	catch (std::exception& e) {
		ConsoleOutput::out_error( e.what() );
		throw;
	}
	catch(...) {
		ConsoleOutput::out_error( "Uncaught unknown exception." );
		throw; //rethrow exception
	}


	// check whether to show help message and exit
	if (vm.count("help")) {
		std::cout << options << std::endl;
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
			szenario_generator generator(vm["seed"].as<unsigned int>());	// set seed
			generator.init(vm["robots"].as<unsigned int>(), vm["algorithm"].as<std::string>());				// init number of robots

			// files
			generator.set_worldFile(vm["swarmfile"].as<std::string>());
			generator.set_robotFile(vm["robotfile"].as<std::string>());
			generator.set_obstacleFile(vm["obstaclefile"].as<std::string>());

			// distribute everything
			if (vm["distr-pos"].as<double>()!=0.0) {
				Vector3d tmpVec;
				tmpVec.insert_element(kXCoord,vm["distr-pos"].as<double>());
				tmpVec.insert_element(kYCoord,vm["distr-pos"].as<double>());
				tmpVec.insert_element(kZCoord,vm["distr-pos"].as<double>());
				generator.distribute_robots_uniform(tmpVec);
			}

			// sets request handler if requested
			if (vm.count("add-pos-handler"))
				generator.add_play_pos_request_handler();
			if (vm.count("add-vel-handler"))
				generator.add_play_vel_request_handler();
			if (vm.count("add-acc-handler"))
				generator.add_play_acc_request_handler();

			// distribute initial velocities
			if (vm["min-vel"].as<double>()!=0.0 || vm["max-vel"].as<double>()!=0.0) {
				generator.distribute_velocity_uniform(vm["min-vel"].as<double>(),vm["max-vel"].as<double>());
			}

			if (vm["min-acc"].as<double>()!=0.0 || vm["max-acc"].as<double>()!=0.0) {
				generator.distribute_acceleration_uniform(vm["min-acc"].as<double>(),vm["max-acc"].as<double>());
			}
			if (vm.count("distr-coord"))
				generator.distribute_coordsys_uniform();

			// write to file
			generator.write_to_file();

			ConsoleOutput::out_info( "Robots were generated!" );
			ConsoleOutput::out_info( "Please see file: " + vm["swarmfile"].as<std::string>() + ".swarm" );
		}
		catch (std::exception& e) {
			ConsoleOutput::out_error( e.what() );
			throw;
		}
		catch(...) {
			ConsoleOutput::out_error( "Uncaught unknown exception." );
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


		// create simulation kernel
		boost::shared_ptr<SimulationControl> sim_control(new SimulationControl());
		sim_control->create_new_simulation(tmpProjectFile,
				                           vm["history-length"].as<unsigned int>(),
				                           vm["output"].as<std::string>(),
				                           create_statistics);

		// setup visualzation
		boost::shared_ptr<GlutVisualizer> visualizer(new GlutVisualizer(*sim_control));
		visualizer->init();
		sim_control->set_visualizer(visualizer);
		Vector3d cam_pos = string_to_vec( sim_control->camera_position() );
		Vector3d cam_dir = string_to_vec( sim_control->camera_direction() );
		visualizer->set_free_cam_para( cam_pos, cam_dir );

		// start simulation
		sim_control->start_simulation();

		// enter visualization's main-loop
		visualizer->glutMainLoop();
	}
	catch(std::exception& e) {
		ConsoleOutput::out_error( e.what() );
		throw; //rethrow exception
	}
	catch(...) {
		ConsoleOutput::out_error( "Uncaught unknown exception." );
		throw; //rethrow exception
	}
	return 0;
}
