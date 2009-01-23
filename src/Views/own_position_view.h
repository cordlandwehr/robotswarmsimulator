/*
 * own_position_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef OWN_POSITION_VIEW_H_
#define OWN_POSITION_VIEW_H_

#include "view.h"

class OwnPositionView: public virtual View {
public:
	OwnPositionView();
	virtual ~OwnPositionView();

protected:
	virtual Vector3d get_own_position(const RobotData& robot) const;
};

#endif /* OWN_POSITION_VIEW_H_ */
