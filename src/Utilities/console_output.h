/*
 * console_output.h
 *
 *  Created on: Mar 22, 2009
 *      Author: phoenixx
 */

#include <string>


#ifndef CONSOLE_OUTPUT_H_
#define CONSOLE_OUTPUT_H_

#include "console_output.h"

class ConsoleOutput {
public:
	static void out_warning(std::string outstring);

	static void out_info(std::string outstring);

	static void out_error(std::string outstring);

	/**
	 * Wraps line with exactly 76 characters
	 * TODO (cola) needs to be implemented
	 */
	static std::string wrap_line(std::string outstring);

	ConsoleOutput();
	virtual ~ConsoleOutput();
};

#endif /* CONSOLE_OUTPUT_H_ */
