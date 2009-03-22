/*
 * time_view.h
 *
 *  Created on: 22.03.2009
 *      Author: Daniel Wonisch
 */

#ifndef TIME_VIEW_H_
#define TIME_VIEW_H_

#include "view.h"

class TimeView: public virtual View {
public:
	TimeView();
	virtual ~TimeView();
protected:
	virtual const int get_time() const;
};

#endif /* TIME_VIEW_H_ */
