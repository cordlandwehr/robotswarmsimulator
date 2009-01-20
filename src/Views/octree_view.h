/*
 * octree_view.h
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#ifndef OCTREE_VIEW_H_
#define OCTREE_VIEW_H_

#include "view.h"

class Octree;

/**
 * \class OctreeView
 * \brief View sub class managing a octree.
 *
 * This class is designed as a base class for view sub classes needing an octree.
 *
 * Abstract class, because it makes no sense to assign this class to a Robot since
 * the view model provided by this class is the same as by View.
 *
 */

class OctreeView: public virtual View {
public:
	OctreeView();
	virtual ~OctreeView() = 0;

	virtual void init(const WorldInformation& world_information);

protected:
	const boost::shared_ptr<Octree>& octree() const;
	boost::shared_ptr<Octree> octree_;
private:

};

#endif /* OCTREE_VIEW_H_ */
