/*
 * own_marker_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef OWN_MARKER_VIEW_H_
#define OWN_MARKER_VIEW_H_

#include "view.h"

class OwnMarkerView: public virtual View {
public:
	OwnMarkerView();
	virtual ~OwnMarkerView();

protected:
	virtual MarkerInformation get_own_marker_information(const RobotData& robot) const;
};

#endif /* OWN_MARKER_VIEW_H_ */
