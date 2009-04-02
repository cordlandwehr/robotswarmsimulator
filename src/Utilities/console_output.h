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

	/**
	 * Creates warning message with given string
	 * @param outstring
	 */
	static void out_warning(std::string outstring);

	/**
	 * Creates warning message with given string
	 * @param outstring
	 */
	static void out_info(std::string outstring);

	/**
	 * Creates warning message with given string
	 * @param outstring
	 */
	static void out_error(std::string outstring);

	/**
	 * Creates warning message with given string
	 * @param outstring
	 */
	static void out_warning(std::string outstring, Module talking_module);

	/**
	 * Creates warning message with given string
	 * @param outstring
	 */
	static void out_info(std::string outstring, Module talking_module);

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
