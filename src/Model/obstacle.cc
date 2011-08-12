/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Sven Kurras <kurras@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>

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

#include "obstacle.h"

Obstacle::Obstacle(boost::shared_ptr<Identifier> id,
                   boost::shared_ptr<Vector3d> position)
        : WorldObject(id, position)
{


}

Obstacle::Obstacle(boost::shared_ptr<Identifier> id,
                   boost::shared_ptr<Vector3d> position,
                   boost::shared_ptr<MarkerInformation> marker_information)
        : WorldObject(id, position, marker_information)
{


}

Obstacle::~Obstacle() {

}
