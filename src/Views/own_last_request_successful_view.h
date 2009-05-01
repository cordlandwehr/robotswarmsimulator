/**
 * \file own_last_request_successful_view.h
 * \author Peter Kling
 * \date 01.05.09
 */

#ifndef OWN_LAST_REQUEST_SUCCESSFUL_VIEW_H_
#define OWN_LAST_REQUEST_SUCCESSFUL_VIEW_H_

#include "view.h"

class OwnLastRequestSuccessfulView: public virtual View {
public:
	OwnLastRequestSuccessfulView();
	virtual ~OwnLastRequestSuccessfulView();
	
protected:
	virtual bool get_own_last_request_successful(const RobotData& robot) const;
};

#endif /* OWN_LAST_REQUEST_SUCCESSFUL_VIEW_H_ */
