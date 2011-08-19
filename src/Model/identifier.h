/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Marcus Märtens <mmarcus@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
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
 * \class	Identifier
 * \author	Martina Hüllmann
 * \brief	Denote ID's of robots.
 */

#ifndef IDENTIFIER_H_
#define IDENTIFIER_H_

#include <cstddef>
#include <boost/shared_ptr.hpp>

// forward declaration for cross namepsace friendship (awww...)
namespace LuaWrapper{class WorldInformationWrapper;}

class Identifier {
public:
	virtual ~Identifier();

	virtual boost::shared_ptr<Identifier> clone() const = 0;


protected:
	explicit Identifier(std::size_t id);
	/**
	 * Return ID of identifier.
	 * \return ID of identifier.
	 */
	std::size_t id() const;

private:
	std::size_t id_;

	// TODO(craupach) this class has many many friends
	friend class View;
	friend class WorldInformation;
	friend class LuaWrapper::WorldInformationWrapper;
	friend class Parser;
	friend class write_obstacle_1;
	friend class SimulationRenderer;
	friend class StatsControl;
	friend class SendMessageRequestHandler;
};

#endif /* IDENTIFIER_H_ */
