/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>
	Copyright (C) 2009-2010  Daniel Wonisch <dwonisch@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Andreas Cord-Landwehr <cola@uni-paderborn.de>
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
 * \brief	TODO insert description here
 */

#ifndef ROBOT_H_
#define ROBOT_H_

#include <set>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class RobotIdentifier;
class View;
class Request;

class Robot {
public:

	explicit Robot(boost::shared_ptr<RobotIdentifier> id) : id_(id) {};
	virtual ~Robot();


	const boost::shared_ptr<RobotIdentifier>& id() const {
		return id_;
	}

	void set_view(const boost::shared_ptr<View>& view);

	const boost::shared_ptr<View> get_view() const;

	virtual std::string get_algorithm_id () const = 0;

	virtual std::set<boost::shared_ptr<Request> > compute() = 0;
protected:
	boost::shared_ptr<View> view_;


private:
	boost::shared_ptr<RobotIdentifier> id_;

};

#endif /* ROBOT_H_ */
