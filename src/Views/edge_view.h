/*
 * edge_view.h
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#ifndef EDGE_VIEW_H_
#define EDGE_VIEW_H_

/**
 * \class EdgeView
 * \brief Robots connected by edges are visible
 *
 * Provides implementations of getVisibleRobots method.
 *
 * Assigning this class to a Robot corresponds to a "local graph view" model.
 *
 */

class EdgeView {
public:
	EdgeView();
	virtual ~EdgeView();

protected:
	virtual std::vector<RobotRef> get_visible_robots(const RobotData& robot) const;
	virtual std::vector<EdgeRef> get_visible_edges(const RobotData& robot) const;
};

#endif /* EDGE_VIEW_H_ */
