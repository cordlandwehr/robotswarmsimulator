/*
 * octree_view.cpp
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#include "octree_view.h"
#include "octree.h"

OctreeView::OctreeView() {
	// TODO Auto-generated constructor stub

}

OctreeView::~OctreeView() {
	// TODO Auto-generated destructor stub
}

void OctreeView::init(const WorldInformation& world_information) {
	View::init(world_information);

	//TODO (dwonisch): Parameters?
	octree_.reset(new Octree());
	//TODO (dwonisch): initialise octree somehow
}

const boost::shared_ptr<Octree>& OctreeView::octree() const {
	return octree_;
}
