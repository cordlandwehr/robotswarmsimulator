/*
 * robot_type_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef ROBOT_TYPE_VIEW_H_
#define ROBOT_TYPE_VIEW_H_

#include "own_type_view.h"

class RobotTypeView: public virtual OwnTypeView {
public:
	RobotTypeView();
	virtual ~RobotTypeView();
protected:
	virtual RobotType get_others_type(const RobotData& robot) const;

};

#endif /* ROBOT_TYPE_VIEW_H_ */
