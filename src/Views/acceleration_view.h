/*
 * acceleration_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef ACCELERATION_VIEW_H_
#define ACCELERATION_VIEW_H_

#include "view.h"

class AccelerationView: public virtual View {
public:
	AccelerationView();
	virtual ~AccelerationView();
protected:
	virtual Vector3d get_own_acceleration(const RobotData& robot) const;
	virtual Vector3d get_others_acceleration(const RobotData& robot) const;
};

#endif /* ACCELERATION_VIEW_H_ */
