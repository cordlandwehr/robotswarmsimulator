/*
 * velocity_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef VELOCITY_VIEW_H_
#define VELOCITY_VIEW_H_

#include "own_velocity_view.h"

class VelocityView: public virtual OwnVelocityView {
public:
	VelocityView();
	virtual ~VelocityView();
protected:
	virtual Vector3d get_others_velocity(const RobotData& robot) const;
};

#endif /* VELOCITY_VIEW_H_ */
