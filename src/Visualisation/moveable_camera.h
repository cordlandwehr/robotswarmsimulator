/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
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

/*
 * MoveableCamera.h
 *
 *  Created on: 18.01.2009
 *      Author: kamil
 */

#ifndef MOVEABLECAMERA_H_
#define MOVEABLECAMERA_H_

#include "../Utilities/vector_arithmetics.h"
#include "camera.h"

/**
 * The MoveableCamera is a first person camera. It allows to navigate in the world freely
 * using the mouse and the keyboard.
 *
 * The important method for navigation are:
 * 	  By mouse:
 * 		set_view_by_mouse to rotate the view by the mouse
 *
 * 	   By keyboard:
 * 		strafe_left, strafe_right, move_up, move_down
 *
 */
class MoveableCamera : public  Camera {

public:
	MoveableCamera();

	/**
	 * \brief This moves the camera's view by the mouse movements (First person view)
	 *
	 * \param x The new mouse position
	 * \param y The new mouse position
	 */
	void set_view_by_mouse(int x, int y);

	void set_button_press_mouse(int x, int y);

	/**
	 * \brief This strafes the camera left or right depending on the speed (+/-)
	 *
	 * \param speed the speed for movement
	 */
	void strafe_camera(float speed);

	/**
	 *  \brief This will move the camera forward or backward depending on the speed
	 * \param speed The speed for movement
	 */
	 void move_camera(float speed);

	 void move_camera_up_down(float speed);

	/**
	 * This will move the camera forward
	 */
	void move_forward();
	/**
	 * This will move the camera backward
	 */
	void move_backward();

	void move_up();

	void move_down();
	/**
	 * This will strafe the camera to the left
	 */
	void strafe_left();
	/**
	 * This will strafe the camera to the right
	 */
	void strafe_right();

	/**
	 * This updates the camera's view and other data.
	 */
	void update(const std::vector<boost::shared_ptr<WorldObject> > & marker,
			const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
			const std::vector<boost::shared_ptr<RobotData> >& robot_data,
			double extrapolate );

	/**
	 * \brief This method lets the maera look at the view point from the position point
	 */
	void look_rot() const;

	void look_translate() const;

	/**
	 * Returns the name of the camera type.
	 */
	virtual std::string get_name();

private:
	int down_x_;
	int down_y_;
};


#endif /* MOVEABLECAMERA_H_ */
