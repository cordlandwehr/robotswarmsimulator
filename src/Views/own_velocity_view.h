/*
 * own_velocity_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef OWN_VELOCITY_VIEW_H_
#define OWN_VELOCITY_VIEW_H_

#include "view.h"

class OwnVelocityView: public virtual View {
public:
	OwnVelocityView();
	virtual ~OwnVelocityView();
protected:
	virtual Vector3d get_own_velocity(const RobotData& robot) const;
};

#endif /* OWN_VELOCITY_VIEW_H_ */
