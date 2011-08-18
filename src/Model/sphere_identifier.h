/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Martina Hüllmann <martinah@uni-paderborn.de>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
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

/**
 * \class	SphereIdentifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of spheres.
 */

#ifndef SPHERE_IDENTIFIER_H_
#define SPHERE_IDENTIFIER_H_

#include "obstacle_identifier.h"

class SphereIdentifier : public ObstacleIdentifier {

	friend class SimpleWorldFixture;

public:
	~SphereIdentifier();

	virtual boost::shared_ptr<Identifier> clone() const {
			return boost::shared_ptr<Identifier>(new SphereIdentifier(*this));
		}
	explicit SphereIdentifier(std::size_t id);
};

#endif /* SPHERE_IDENTIFIER_H_ */
