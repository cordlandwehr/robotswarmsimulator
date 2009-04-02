/*
 * neighbor_view.h
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#ifndef NEIGHBOR_VIEW_H_
#define NEIGHBOR_VIEW_H_

#include "octree_view.h"

/**
 * \class NeighborView
 * \brief Basic k-nearest-neighbor view model.
 *
 * Provides implementations of getVisible... methods. To efficiently implement them
 * the classes uses an Octree as internal data structure.
 *
 * Assigning this class to a Robot corresponds to a "see the k nearest objects" model.
 *
 */
class NeighborView : public virtual OctreeView {
public:
	NeighborView(unsigned seen_objects_count);
	virtual ~NeighborView();

protected:
	virtual std::vector<RobotRef> get_visible_robots(const RobotData& robot) const;
	virtual std::vector<ObstacleRef> get_visible_obstacles(const RobotData& robot) const;
	virtual std::vector<MarkerRef> get_visible_markers(const RobotData& robot) const;

	unsigned seen_objects_count() const;

private:
	unsigned seen_objects_count_;
};

#endif /* NEIGHBOR_VIEW_H_ */
