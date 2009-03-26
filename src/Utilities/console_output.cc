/*
 * console_output.cc
 *
 *  Created on: Mar 22, 2009
 *      Author: phoenixx
 */

#include "console_output.h"
#include <string>
#include <iostream>

void ConsoleOutput::out_warning(std::string outstring, Module talking_module) {


	std::cout << "(WW) "<< get_prefix(talking_module) << wrap_line(outstring) << std::endl;
}

void ConsoleOutput::out_info(std::string outstring, Module talking_module) {
	std::cout << "(II) " << get_prefix(talking_module) << wrap_line(outstring) << std::endl;
}

void ConsoleOutput::out_error(std::string outstring, Module talking_module) {
	std::cerr << "(EE) " << get_prefix(talking_module) << outstring << std::endl;
}

void ConsoleOutput::out_warning(std::string outstring) {
	out_warning(outstring, None);
}

void ConsoleOutput::out_info(std::string outstring) {
	out_info(outstring, None);
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
