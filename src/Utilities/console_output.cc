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
	std::fstream logfile_stream(NULL, std::fstream::out);
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
	if (level == DEBUG) {
		*output_stream << "[DEBUG]";
	} else if (level == INFO) {
		*output_stream << "[INFO]";
	} else if (level == WARNING) {
		*output_stream << "[WARNING]";
	} else if (level == ERROR) {
		*output_stream << "[ERROR]";
	}
	*output_stream << get_prefix(talking_module);

	// return stream
	return *output_stream;

}

void ConsoleOutput::out_error(std::string outstring, Module talking_module) {
	std::cerr << "(EE) " << get_prefix(talking_module) << outstring << std::endl;
}

void ConsoleOutput::out_error(std::string outstring) {
	out_error(outstring, None);
}

std::string ConsoleOutput::get_prefix(Module talking_module) {
	std::string prefix = "";
	switch (talking_module) {
	case Visualization: prefix = "[VISUALIZATION] "; break;
	case Control: prefix = "[SIMCONTROL] "; break;
	case Statistics: prefix = "[STATISTICS] "; break;
	case Parser: prefix = "[PARSER] "; break;
	case Kernel: prefix = "[KERNEL] "; break;
	case View: prefix = "[VIEW] "; break;
	default : prefix = ""; break;
	}
	return prefix;
}

std::string ConsoleOutput::wrap_line(std::string outstring) {
	return outstring;
}

ConsoleOutput::ConsoleOutput() {
	// TODO Auto-generated constructor stub
}

ConsoleOutput::~ConsoleOutput() {
	// TODO Auto-generated destructor stub
}
