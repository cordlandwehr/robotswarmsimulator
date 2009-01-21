/*
 * MoveableCamera.h
 *
 *  Created on: 18.01.2009
 *      Author: kamil
 */

#ifndef MOVEABLECAMERA_H_
#define MOVEABLECAMERA_H_




#include "../Utilities/vector3d.h"
#include "camera.h"

#define kSpeed	0.1f


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




	/**
	 * This will move the camera forward
	 */
	void move_forward();
	/**
	 * This will move the camera backward
	 */
	void move_backward();
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
			const std::vector<boost::shared_ptr<RobotData> >& robot_data);



	/**
	 * \brief This method lets the maera look at the view point from the position point
	 */
	void look() const;

};


#endif /* MOVEABLECAMERA_H_ */