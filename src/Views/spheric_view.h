/*
 * spheric_view.h
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#ifndef SPHERIC_VIEW_H_
#define SPHERIC_VIEW_H_

#include "octree_view.h"
#include "../Model/identifier.h"

/**
 * \class SphericView
 * \brief Basic spherical view model.
 *
 * Provides implementations of getVisible... methods. To efficiently implement them
 * the classes uses an Octree as internal data structure.
 *
 * Assigning this class to a Robot corresponds to a "see all objects in a given radius" model.
 *
 */

class SphericView : public virtual OctreeView {
public:
	SphericView(double view_radius);
	virtual ~SphericView();
	void init(const boost::shared_ptr<WorldInformation>& world_information );

protected:
	virtual std::vector<RobotRef> get_visible_robots(const RobotData& robot) const;
	virtual std::vector<ObstacleRef> get_visible_obstacles(const RobotData& robot) const;
	virtual std::vector<MarkerRef> get_visible_markers(const RobotData& robot) const;

	double view_radius() const;

private:
	double view_radius_;
};

#endif /* SPHERIC_VIEW_H_ */
