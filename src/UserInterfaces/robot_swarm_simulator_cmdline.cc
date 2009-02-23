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

#include <boost/shared_ptr.hpp>
#include <boost/program_options.hpp>

#include <config.h>
#include <SimulationControl/simulation_control.h>
#include <UserInterfaces/glut_visualizer.h>
#include <Utilities/szenario_generator.h>


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
		("worldfile", po::value<std::string>()->default_value("newrandom"), "world-file for output")
		("robotfile", po::value<std::string>()->default_value("newrandom"), "robot-file for output")
		("obstaclefile", po::value<std::string>()->default_value("newrandom"), "obstacle-file for output")
		("distr-pos", po::value<double>()->default_value(0), "distribute velocity in cube [0;distr-pos]^3")
		("distr-vel", po::value<double>()->default_value(0), "distribute velocity in cube [0;distr-vel]^3")
		("distr-acc", po::value<double>()->default_value(0), "distribute velocity in cube [0;distr-acc]^3")
		("distr-coord", "distribute robot coordsystems uniformly");

	po::options_description simulation_options("Simulation options");
	simulation_options.add_options()
		("project-file", po::value<std::string>(), "Project file to load")
		("history-length", po::value<unsigned int>()->default_value(25), "history length");

	po::options_description options;
	options.add(general_options).add(generation_options).add(simulation_options);

	// create variable map and parse options from command line
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, options), vm);
	po::notify(vm);

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

		// init
		szenario_generator generator(vm["seed"].as<unsigned int>());	// set seed
		generator.init(vm["robots"].as<unsigned int>(), vm["algorithm"].as<std::string>());				// init number of robots

		// files
		generator.set_worldFile(vm["worldfile"].as<std::string>());
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
		if (vm["distr-vel"].as<double>()!=0.0) {
			Vector3d tmpVec;
			tmpVec.insert_element(kXCoord,vm["distr-vel"].as<double>());
			tmpVec.insert_element(kYCoord,vm["distr-vel"].as<double>());
			tmpVec.insert_element(kZCoord,vm["distr-vel"].as<double>());
			generator.distribute_velocity_uniform(tmpVec);
		}
		if (vm["distr-acc"].as<double>()!=0.0) {
			Vector3d tmpVec;
			tmpVec.insert_element(kXCoord,vm["distr-acc"].as<double>());
			tmpVec.insert_element(kYCoord,vm["distr-acc"].as<double>());
			tmpVec.insert_element(kZCoord,vm["distr-acc"].as<double>());
			generator.distribute_acceleration_uniform(tmpVec);
		}
		if (vm.count("distr-coord"))
			generator.distribute_coordsys_uniform();

		// write to file
		generator.write_to_file();

		std::cout << "Robots were generated!" << std::endl;
		std::cout << "Please see file: " + vm["worldfile"].as<std::string>() + ".swarm" << std::endl << std::endl;

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

		// create simulation kernel
		boost::shared_ptr<SimulationControl> sim_control(new SimulationControl());
		sim_control->create_new_simulation(tmpProjectFile, vm["history-length"].as<unsigned int>());

		// setup visualzation
		boost::shared_ptr<GlutVisualizer> visualizer(new GlutVisualizer(*sim_control));
		visualizer->init();
		sim_control->set_visualizer(visualizer);

		// start simulation
		sim_control->start_simulation();

		// enter visualization's main-loop
		visualizer->glutMainLoop();
	}
	catch(std::exception& e) {
		std::cerr << "Uncaught exception: " << e.what() << std::endl;
		throw; //rethrow exception
	}
	catch(...) {
		std::cerr << "Uncaught, unknown exception." << std::endl;
		throw; //rethrow exception
	}
	return 0;
}
