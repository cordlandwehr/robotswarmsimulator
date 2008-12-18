//============================================================================
// Name        : RobotSwarmSimulator.cpp
// Author      : Christoph Raupach
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "testheader.h"
#include "Flags/gflags.h"
#include "Logger/logging.h"

using namespace std;

int main(int argc, char *argv[]) {
  // initalize flags and logging
  google::ParseCommandLineFlags(&argc, &argv, true);
  init_logging_system();
  
	log(INFO) << "Hello swarm!" << endl; // prints Hello swarm!
	Test test;
	test.say_hello();
	return 0;
}
