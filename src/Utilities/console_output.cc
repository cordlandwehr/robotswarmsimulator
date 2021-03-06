/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>

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
 * console_output.cc
 *
 *  Created on: Mar 22, 2009
 *      Author: phoenixx
 */

#include "console_output.h"
#include <string>
#include <iostream>
#include <fstream>

namespace {
	// the level on which messages should be logged
	ConsoleOutput::Level level_;

	// true iff output should be directed into a file
	bool log_to_file_;

	NullStream global_null_stream;
	std::fstream logfile_stream("", std::fstream::out);
}

void ConsoleOutput::initalize_logging_system(Level level, bool log_to_file, std::string filename) {
	level_ = level;
	log_to_file_ = log_to_file;
	if(log_to_file) {
		if(filename == "") {
			// error!
			std::cout << "[ERROR] Logging System intialize failed: Filename empty, but want to log to file." << std::endl;
			std::cout << "logging to std::cout instead..." << std::endl;
			log_to_file_ = false;
			return;
		}

		// initialize stream to log file
		logfile_stream.close();
		logfile_stream.open(filename.c_str(), std::fstream::out);
	}
}

std::ostream & ConsoleOutput::log(Module talking_module, Level level) {
	// don't log if level not high enough
	if(level < level_) {
		return global_null_stream;
	}

	// build the output stream (either to stdout or to a file)
	std::ostream * output_stream;
	if(log_to_file_) {
		output_stream = &logfile_stream;
	} else {
		output_stream = &std::cout;
	}

	// start a new line, log the level, log the module of the message
	*output_stream << std::endl;
	if (level == debug) {
		*output_stream << "[DEBUG]";
	} else if (level == info) {
		*output_stream << "[INFO]";
	} else if (level == warning) {
		*output_stream << "[WARNING]";
	} else if (level == error) {
		*output_stream << "[ERROR]";
	}
	*output_stream << get_prefix(talking_module);

	// return stream
	return *output_stream;

}

std::string ConsoleOutput::get_prefix(Module talking_module) {
	std::string prefix = "";
	switch (talking_module) {
	case Visualization: prefix = "[VISUALIZATION] "; break;
	case EventHandler: prefix = "[EVENTHANDLER] "; break;
	case Control: prefix = "[SIMCONTROL] "; break;
	case Statistics: prefix = "[STATISTICS] "; break;
	case Parser: prefix = "[PARSER] "; break;
	case Kernel: prefix = "[KERNEL] "; break;
	case View: prefix = "[VIEW] "; break;
	case Lua: prefix = "[LUA] "; break;
	default : prefix = ""; break;
	}
	return prefix;
}

ConsoleOutput::ConsoleOutput() {
	// TODO Auto-generated constructor stub
}

ConsoleOutput::~ConsoleOutput() {
	// TODO Auto-generated destructor stub
}
