//
//  console_output_wrapper.cc
//  Robot Swarm Simulator
//
//  Created by Michael Knopf on 18.08.11.
//

#include <boost/algorithm/string.hpp>

#include "../Utilities/console_output.h"

#include "console_output_wrapper.h"

ConsoleOutputWrapper::ConsoleOutputWrapper ()
{

}

ConsoleOutputWrapper::~ConsoleOutputWrapper ()
{

}

void
ConsoleOutputWrapper::log (const std::string& message)
{
  log("none", message);
}

void
ConsoleOutputWrapper::log (const std::string& level, const std::string& message)
{
  std::string level_lower(level);
  boost::to_lower(level_lower);
  ConsoleOutput::Level level_enum;
  
  if (level_lower == "debug") {
    level_enum = ConsoleOutput::debug;
  } else if (level_lower == "info") {
    level_enum = ConsoleOutput::info;
  } else if (level_lower == "warning") {
    level_enum = ConsoleOutput::warning;
  } else if (level_lower == "error") {
    level_enum = ConsoleOutput::error;
  } else {
    level_enum = ConsoleOutput::none;
  } 
  
  ConsoleOutput::log(ConsoleOutput::Lua, level_enum) << message;
}
