/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

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

#ifndef UNSUPPORTED_OPERATION_EXCEPTION_H_
#define UNSUPPORTED_OPERATION_EXCEPTION_H_

#include <stdexcept>
#include <string>
#include "console_output.h"

class UnsupportedOperationException : public std::runtime_error {
public:
	UnsupportedOperationException(const std::string& msg) : std::runtime_error(msg) {
		;
	}
	virtual ~UnsupportedOperationException() throw () {
		;
	}
};

#endif /* UNSUPPORTED_OPERATION_EXCEPTION_H_ */
