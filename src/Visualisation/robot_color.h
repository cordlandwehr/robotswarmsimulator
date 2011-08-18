/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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
 * robot_color.h
 *
 *  Created on: 20.02.2009
 *      Author: kamil
 */

#ifndef ROBOT_COLOR_H_
#define ROBOT_COLOR_H_

const unsigned int kRobotIdColorNum = 10;

/**
 * A list of colors to map color numbers to RGB triples
 */
const float kRobotIdColor [][3] = {
	{ 0.0f,1.0f,0.0f },
	{ 0.0f,0.0f,1.0f },
	{ 0.0f,1.0f,1.0f },
	{ 1.0f,0.0f,0.0f },
	{ 1.0f,0.0f,1.0f },
	{ 1.0f,1.0f,0.0f },
	{ 1.0f,1.0f,1.0f },
	{ 0.0f,0.0f,0.0f },
	{ 1.0f,0.5f,0.0f },
	{ 0.5f,0.0f,1.0f },
};

#endif /* ROBOT_COLOR_H_ */
