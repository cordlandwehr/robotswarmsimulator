/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>

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

	virtual std::vector<View::EdgeRef> get_visible_edges(const RobotData& robot) const;
	virtual bool is_edge_directed(const Edge& edge) const;
	virtual View::RobotRef get_edge_source(const Edge& edge) const;
	virtual View::RobotRef get_edge_target(const Edge& edge) const;
};

#endif /* FULL_VIEW_H_ */
