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

class FollowSwarmCamera : public Camera{

public:
	FollowSwarmCamera();

	/**
	 * \brief Updates the view to display the whole swarm
	 */
	virtual void update(const std::vector<boost::shared_ptr<WorldObject> > & marker,
						const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
						const std::vector<boost::shared_ptr<RobotData> >& robot_data,
						double extrapolate);


	/**
	 * \brief This method lets the maera look at the view point from the position point
	 */
	void look() const;


};

#endif /* FOLLOW_SWARM_CAMERA_H_ */
