/*
 * edge_view.h
 *
 *  Created on: 27.07.2011
 *      Author: Jonas
 */

#ifndef EDGE_VIEW_H_
#define EDGE_VIEW_H_

#include "view.h"

/**
 * \class EdgeView
 * \brief Robots connected by edges are visible
 *
 * Provides implementations of getVisibleRobots method.
 *
 * Assigning this class to a Robot corresponds to a "local graph view" model.
 *
 */

class EdgeView : public virtual View {
public:
	EdgeView();
	virtual ~EdgeView();

protected:
	virtual std::vector<View::RobotRef> get_visible_robots(const RobotData& robot) const;
	virtual std::vector<View::EdgeRef> get_visible_edges(const RobotData& robot) const;
	virtual bool is_edge_directed(boost::shared_ptr<EdgeIdentifier> e_id) const;
};

#endif /* EDGE_VIEW_H_ */
