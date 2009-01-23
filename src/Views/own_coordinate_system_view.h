/*
 * own_coordinate_system_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef OWN_COORDINATE_SYSTEM_VIEW_H_
#define OWN_COORDINATE_SYSTEM_VIEW_H_

#include "view.h"

class OwnCoordinateSystemView: public virtual View {
public:
	OwnCoordinateSystemView();
	virtual ~OwnCoordinateSystemView();
protected:
	virtual boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > get_own_coordinate_system_axis(const RobotData& robot) const;
};

#endif /* OWN_COORDINATE_SYSTEM_VIEW_H_ */
