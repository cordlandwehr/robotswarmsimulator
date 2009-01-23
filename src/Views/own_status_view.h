/*
 * own_status_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef OWN_STATUS_VIEW_H_
#define OWN_STATUS_VIEW_H_

#include "view.h"

class OwnStatusView: public virtual View {
public:
	OwnStatusView();
	virtual ~OwnStatusView();
protected:
	virtual RobotStatus get_own_status(const RobotData& robot) const;
};

#endif /* OWN_STATUS_VIEW_H_ */
