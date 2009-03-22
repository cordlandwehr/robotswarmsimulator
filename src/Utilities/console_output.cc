/*
 * console_output.cc
 *
 *  Created on: Mar 22, 2009
 *      Author: phoenixx
 */

#include "console_output.h"
#include <string>
#include <iostream>

void ConsoleOutput::out_warning(std::string outstring) {
	std::cout << " (WW) " << wrap_line(outstring) << std::endl;
}

void ConsoleOutput::out_info(std::string outstring) {
	std::cout << " (II) " << wrap_line(outstring) << std::endl;
}

void ConsoleOutput::out_error(std::string outstring) {
	std::cerr << " (EE) " << outstring << std::endl;
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
