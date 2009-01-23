/*
 * own_type_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef OWN_TYPE_VIEW_H_
#define OWN_TYPE_VIEW_H_

#include "view.h"

class OwnTypeView: public virtual View {
public:
	OwnTypeView();
	virtual ~OwnTypeView();
protected:
	virtual RobotType get_own_type(const RobotData& robot) const;
};

#endif /* OWN_TYPE_VIEW_H_ */
