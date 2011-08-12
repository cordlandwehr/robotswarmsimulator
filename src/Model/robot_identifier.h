/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
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

/**
 * \class	RobotIdentifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of robots.
 */

#ifndef ROBOT_IDENTIFIER_H_
#define ROBOT_IDENTIFIER_H_

#include "identifier.h"




class RobotIdentifier : public Identifier {
public:
	explicit RobotIdentifier(std::size_t id);
	~RobotIdentifier();

	virtual boost::shared_ptr<Identifier> clone() const {
		return boost::shared_ptr<Identifier>(new RobotIdentifier(*this));
	}


};



#endif /* ROBOT_IDENTIFIER_H_ */
