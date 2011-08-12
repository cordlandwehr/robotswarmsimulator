/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>

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
 * color_change_request.h
 *
 *  Created on: 02.07.2009
 *      Author: kamil
 */

#ifndef COLOR_CHANGE_REQUEST_H_
#define COLOR_CHANGE_REQUEST_H_

#include "request.h"


class ColorChangeRequest : public Request {
public:
	ColorChangeRequest(Robot & robot, unsigned int color) :
	    Request(robot),
	    requested_color_(color) {}

	unsigned int requested_color() const { return requested_color_; }

private:
	unsigned int requested_color_;
};


#endif /* COLOR_CHANGE_REQUEST_H_ */
