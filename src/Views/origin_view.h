/*
 * origin_view.h
 *
 *  Created on: 22.03.2009
 *      Author: Daniel Wonisch
 */

#ifndef ORIGIN_VIEW_H_
#define ORIGIN_VIEW_H_

#include "own_origin_view.h"

class OriginView : public virtual OwnOriginView {
public:
	OriginView();
	virtual ~OriginView();
protected:
	virtual Vector3d get_others_coordinate_system_origin(const RobotData& robot) const;
};

#endif /* ORIGIN_VIEW_H_ */
