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
		EventHandler,
		Control,
		Statistics,
		Parser,
		Kernel,
		View,
		ComputationalGeometry
	};

	enum Level {
		debug = 0,
		info = 1,
		warning = 2,
		error = 3,
		none = 4
	};

	static void initalize_logging_system(Level level, bool log_to_file, std::string filename = "");

	static std::ostream & log(Module talking_module, Level level);

	/**
	 * Gives the prefix for selected module for output
	 * @param talking_module of enum type Module
	 */
	static std::string get_prefix(Module talking_module);

	ConsoleOutput();
	virtual ~ConsoleOutput();
};

#endif /* CONSOLE_OUTPUT_H_ */
