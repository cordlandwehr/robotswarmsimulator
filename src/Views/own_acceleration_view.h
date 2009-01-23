/*
 * own_acceleration_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef OWN_ACCELERATION_VIEW_H_
#define OWN_ACCELERATION_VIEW_H_

#include "view.h"

class OwnAccelerationView: public virtual View {
public:
	OwnAccelerationView();
	virtual ~OwnAccelerationView();
protected:
	virtual Vector3d get_own_acceleration(const RobotData& robot) const;
};

#endif /* OWN_ACCELERATION_VIEW_H_ */
