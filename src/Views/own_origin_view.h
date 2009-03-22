/*
 * own_origin_view.h
 *
 *  Created on: 22.03.2009
 *      Author: Daniel Wonisch
 */

#ifndef OWN_ORIGIN_VIEW_H_
#define OWN_ORIGIN_VIEW_H_

#include "view.h"

class OwnOriginView: public virtual View {
public:
	OwnOriginView();
	virtual ~OwnOriginView();
protected:
	virtual Vector3d get_own_coordinate_system_origin(const RobotData& robot) const;
};

#endif /* OWN_ORIGIN_VIEW_H_ */
