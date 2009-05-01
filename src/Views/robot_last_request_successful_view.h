/**
 * \file robot_last_request_successful_view.h
 * \author Peter Kling
 * \date 01.05.09
 */

#ifndef ROBOT_LAST_REQUEST_SUCCESSFUL_VIEW_H_
#define ROBOT_LAST_REQUEST_SUCCESSFUL_VIEW_H_

#include "own_last_request_successful_view.h"

class RobotLastRequestSuccessfulView: public virtual OwnLastRequestSuccessfulView {
public:
	RobotLastRequestSuccessfulView();
	virtual ~RobotLastRequestSuccessfulView();
	
protected:
	virtual bool get_others_last_request_successful(const RobotData& robot) const;
};

#endif /* ROBOT_LAST_REQUEST_SUCCESSFUL_VIEW_H_ */
