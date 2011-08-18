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

	virtual void init(const boost::shared_ptr<WorldInformation>& world_information);

protected:
	const boost::shared_ptr<const Octree> octree() const;
	boost::shared_ptr<Octree> octree_;
private:

};

#endif /* OCTREE_VIEW_H_ */
