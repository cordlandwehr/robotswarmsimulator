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
 * follow_swarm_camera.h
 *
 *  Created on: 18.01.2009
 *      Author: kamil
 */

#ifndef FOLLOW_SWARM_CAMERA_H_
#define FOLLOW_SWARM_CAMERA_H_

#include "camera.h"

class WorldObject;
class Obstacle;
class RobotData;

/**
 * The FollowSwarmCamera shows all robots of the swarm.
 * It calculates the bounding box of all robots and sets its position and
 * look at vector such that the bounding box would be visible.
 *
 * Usage:
 *  Like the Camera class.
 *
 */
class FollowSwarmCamera : public Camera {

public:
	FollowSwarmCamera();

	/**
	 * \brief Updates the view to display the whole swarm
	 */
	void update(const std::vector<boost::shared_ptr<WorldObject> > & marker,
						const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
						const std::vector<boost::shared_ptr<RobotData> >& robot_data,
						double extrapolate);

	/**
	 * \brief This method lets the maera look at the view point from the position point
	 */
	void look_rot() const;
	void look_translate() const;

	/**
	 * Returns the name of the camera type.
	 */
	virtual std::string get_name();
};

#endif /* FOLLOW_SWARM_CAMERA_H_ */
