/*
 * octree_view.cpp
 *
 *  Created on: 15.01.2009
 *      Author: Daniel
 */

#include "../Model/world_information.h"
#include "octree_view.h"
#include "octree.h"

OctreeView::OctreeView() {


}

OctreeView::~OctreeView() {

}

void OctreeView::init(const WorldInformation& world_information) {
	View::init(world_information);

	//TODO (dwonisch): Parameters?
	octree_.reset(new Octree(8,5.0));
	octree_->create_tree(world_information.markers(),
						world_information.obstacles(),
						world_information.robot_data() );
}

const boost::shared_ptr<Octree>& OctreeView::octree() const {
	return octree_;
}
