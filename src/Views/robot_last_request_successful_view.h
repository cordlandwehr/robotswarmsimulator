/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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
 * \file robot_last_request_successful_view.h
 * \author Peter Kling
 * \date 01.05.09
 */

#ifndef ROBOT_LAST_REQUEST_SUCCESSFUL_VIEW_H_
#define ROBOT_LAST_REQUEST_SUCCESSFUL_VIEW_H_

#include "own_last_request_successful_view.h"

class RobotLastRequestSuccessfulView: public virtual OwnLastRequestSuccessfulView {
public:
	RobotLastRequestSuccessfulView();
	virtual ~RobotLastRequestSuccessfulView();
	
protected:
	virtual bool get_others_last_request_successful(const RobotData& robot) const;
};

#endif /* ROBOT_LAST_REQUEST_SUCCESSFUL_VIEW_H_ */
