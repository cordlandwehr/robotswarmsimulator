/*
 * coordinate_system_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef COORDINATE_SYSTEM_VIEW_H_
#define COORDINATE_SYSTEM_VIEW_H_

#include "view.h"

class CoordinateSystemView: public virtual View {
public:
	CoordinateSystemView();
	virtual ~CoordinateSystemView();
protected:
	virtual boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > get_own_coordinate_system_axis(const RobotData& robot) const;
	virtual boost::tuple<boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d>,boost::shared_ptr<Vector3d> > get_others_coordinate_system_axis(const RobotData& robot) const;

};

#endif /* COORDINATE_SYSTEM_VIEW_H_ */
