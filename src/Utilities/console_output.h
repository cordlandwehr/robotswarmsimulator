/*
 * console_output.h
 *
 *  Created on: Mar 22, 2009
 *      Author: phoenixx
 */

#ifndef CONSOLE_OUTPUT_H_
#define CONSOLE_OUTPUT_H_

#include <string>
#include <fstream>

/*
 * The NullStream class sends incoming log messages into the vast nothingness
 */
class NullStream : public std::ostream {
  public:
    // constructor to enable constructing the class without parameters
    NullStream():std::ostream(NULL) {};

    // override of operator << to ignore the parameter
    template<typename T>
    std::ostream& operator<< (const T& t) {
      return *this;
    }
};

class ConsoleOutput {
public:
	enum Module {
		None,
		Visualization,
		Control,
		Statistics,
		Parser,
		Kernel,
		View,
		ComputationalGeometry
	};

	enum Level {
		DEBUG = 0,
		INFO = 1,
		WARNING = 2,
		ERROR = 3,
		NONE = 4
	};

	static void initalize_logging_system(Level level, bool log_to_file, std::string filename = "");

	static std::ostream & log(Module talking_module, Level level);

	/**
	 * Creates warning message with given string
	 * @param outstring
	 */
	static void out_error(std::string outstring);

	/**
	 * Creates warning message with given string
	 * @param outstring
	 */
	static void out_error(std::string outstring, Module talking_module);

	/**
	 * Gives the prefix for selected module for output
	 * @param talking_module of enum type Module
	 */
	static std::string get_prefix(Module talking_module);

	/**
	 * Wraps line with exactly 76 characters
	 * TODO (cola) needs to be implemented
	 */
	static std::string wrap_line(std::string outstring);

	ConsoleOutput();
	virtual ~ConsoleOutput();
};

#endif /* CONSOLE_OUTPUT_H_ */
