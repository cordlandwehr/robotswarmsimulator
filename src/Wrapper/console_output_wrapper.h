//
//  console_output_wrapper.h
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 18.08.11.
//


#ifndef CONSOLE_OUTPUT_WRAPPER_H
#define CONSOLE_OUTPUT_WRAPPER_H

#include <string>

class ConsoleOutputWrapper
{

public:
  ConsoleOutputWrapper ();
  virtual ~ ConsoleOutputWrapper ();
  
  static void log(const std::string& message);
  static void log(const std::string& level, const std::string& message);
};

#endif // CONSOLE_OUTPUT_WRAPPER_H
