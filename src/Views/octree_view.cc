/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>

	RobotSwarmSimulator is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	RobotSwarmSimulator is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with RobotSwarmSimulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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

void OctreeView::init(const boost::shared_ptr<WorldInformation>& world_information) {
	View::init(world_information);

	//TODO (dwonisch): maybe add cache (static map: WorldInformation* -> Octree) to avoid unnecessary copies.
	octree_.reset(new Octree(8,5.0));
	octree_->create_tree(world_information->markers(),
						world_information->obstacles(),
						world_information->robot_data() );
}

const boost::shared_ptr<const Octree> OctreeView::octree() const {
	return octree_;
}
