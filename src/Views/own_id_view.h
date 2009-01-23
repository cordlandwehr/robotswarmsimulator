/*
 * own_id_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef OWN_ID_VIEW_H_
#define OWN_ID_VIEW_H_

#include "view.h"

class OwnIdView: public virtual View {
public:
	OwnIdView();
	virtual ~OwnIdView();
protected:
	virtual std::size_t get_own_id(const RobotData& robot) const;
};

#endif /* OWN_ID_VIEW_H_ */
