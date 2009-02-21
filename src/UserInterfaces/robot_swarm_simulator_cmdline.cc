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

#include <SimulationControl/simulation_control.h>
#include <UserInterfaces/glut_visualizer.h>


int main(int argc, char** argv) {
	namespace po = boost::program_options;

	// declaration of program options
	po::options_description general_options("General options");
	general_options.add_options()
		("help", "shows this help message");

	po::options_description simulation_options("Simulation options");
	simulation_options.add_options()
		("project-file", po::value<std::string>(), "Project file to load")
		("history-length", po::value<unsigned int>()->default_value(25), "history length");

	po::options_description options;
	options.add(general_options).add(simulation_options);

	// create variable map and parse options from command line
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, options), vm);
	po::notify(vm);

	// check whether to show help message and exit
	if (vm.count("help")) {
		std::cout << options << std::endl;
		return 1;
	}

	// make sure we got a project file as input
	if (!vm.count("project-file")) {
		std::cout << "Usage: '" << argv[0] << " --project-file <filename>'" << std::endl;
		std::cout << "   or: '" << argv[0] << " --help' for additional options" << std::endl;
		return 1;
	}
	try {
		// create simulation kernel
		boost::shared_ptr<SimulationControl> sim_control(new SimulationControl());
		sim_control->create_new_simulation(vm["project-file"].as<std::string>(), vm["history-length"].as<unsigned int>());

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
