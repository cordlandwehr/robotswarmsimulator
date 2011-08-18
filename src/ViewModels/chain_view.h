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
 * chain_view.h
 *
 *  Created on: 23.01.2009
 *      Author: Life
 */

#ifndef CHAIN_VIEW_H_
#define CHAIN_VIEW_H_

#include "../Views/position_view.h"
#include "../Views/neighbor_view.h"

/**
 * \class ChainView
 * \brief View model of the robot chain algorithm
 *
 * Assigning this class to a Robot corresponds to the robot chain view model, i.e. every Robot can see k neighbor Robots
 * position. Besides this no more information is visible.
 *
 * @see https://wiki.math.uni-paderborn.de/pg-schwarm/StartSeite/AK/Szenarien
 *
 */

class ChainView: public virtual PositionView, public virtual NeighborView {
public:
	ChainView(unsigned k);
	virtual ~ChainView();
};

#endif /* CHAIN_VIEW_H_ */
