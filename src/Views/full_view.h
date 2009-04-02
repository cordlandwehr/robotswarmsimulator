/*
 * full_view.h
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#ifndef FULL_VIEW_H_
#define FULL_VIEW_H_

#include "view.h"

/**
 * \class FullView
 * \brief All objects visible to all robots view model.
 *
 * Provides implementations of getVisible... methods.
 *
 * Assigning this class to a Robot corresponds to a "all objects visible to all robots" model.
 *
 */
class FullView : public virtual View {
public:
	FullView();
	virtual ~FullView();

protected:
	virtual std::vector<RobotRef> get_visible_robots(const RobotData& robot) const;
	virtual std::vector<ObstacleRef> get_visible_obstacles(const RobotData& robot) const;
	virtual std::vector<MarkerRef> get_visible_markers(const RobotData& robot) const;
};

#endif /* FULL_VIEW_H_ */
