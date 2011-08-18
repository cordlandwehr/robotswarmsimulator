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
		ComputationalGeometry,
		Lua
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
