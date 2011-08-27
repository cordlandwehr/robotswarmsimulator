/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>

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
 * visualizer.h
 *
 *  Created on: 19.01.2009
 *      Author: Daniel
 */

#ifndef VISUALIZER_H_
#define VISUALIZER_H_

class TimePoint;

/**
 * \brief Interface for visualization classes.
 */
class Visualizer {
public:
	virtual ~Visualizer() {}

	/**
	 * \brief Visualization classes have to overwrite this method to render the given world information.
	 *
	 * The given world information may be extrapolated using the given extrapolation time.
	 */
	virtual void draw(double extrapolation_time, const boost::shared_ptr<TimePoint> & time_point) = 0;

	/**
	 * \brief To be used for initialization.
	 */
	virtual void init() = 0;
	virtual void resize(int x, int y) = 0;
	virtual void mouse_func(int button, int state, int x, int y) = 0;
	//virtual void keyboard_func(unsigned char key, int x, int y) = 0;
};

#endif /* VISUALIZER_H_ */
