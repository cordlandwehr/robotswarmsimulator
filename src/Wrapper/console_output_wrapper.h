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
  
  /**
   * Log an info level message
   * 
   * \param message	String containing the message to be logged.
   */
  static void log(const std::string& message);
  /**
   * Log a message with a given level.
   * 
   * \param level	Level of the message (debug, info, warning, error).
   * \param message	String containing the message to be logged.
   */
  static void log(const std::string& level, const std::string& message);
};

#endif // CONSOLE_OUTPUT_WRAPPER_H
