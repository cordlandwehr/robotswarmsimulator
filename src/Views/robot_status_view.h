/*
 * robot_status_view.h
 *
 *  Created on: 20.01.2009
 *      Author: Daniel
 */

#ifndef ROBOT_STATUS_VIEW_H_
#define ROBOT_STATUS_VIEW_H_

#include "view.h"

class RobotStatusView: public virtual View {
public:
	RobotStatusView();
	virtual ~RobotStatusView();
protected:
	virtual RobotStatus get_own_status(const RobotData& robot) const;
	virtual RobotStatus get_others_status(const RobotData& robot) const;
};

#endif /* ROBOT_STATUS_VIEW_H_ */
