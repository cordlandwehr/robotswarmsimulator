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
class FollowSwarmCamera : public Camera{

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


};

#endif /* FOLLOW_SWARM_CAMERA_H_ */
