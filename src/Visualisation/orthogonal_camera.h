/*
 * orthogonal_camera.h
 *
 *  Created on: 09.09.2011
 *      Author: Sascha Brandt
 */

#ifndef ORTHOGONAL_CAMERA_H_
#define ORTHOGONAL_CAMERA_H_

#include "camera.h"

/**
 * The OrthogonalCamera is a orthogonal camera. It allows to navigate in a 2d
 * representation of the world freely using the mouse and the keyboard.
 *
 * The important method for navigation are:
 * 	  By mouse:
 * 		set_view_by_mouse to move the view by the mouse
 *
 * 	   By keyboard:
 * 		strafe_left, strafe_right, move_up, move_down
 *
 * @note move_forward (move_backward) zooms the 2d view in (out).
 *
 */
class OrthogonalCamera: public Camera {
public:
	enum { X_AXIS, Y_AXIS, Z_AXIS };

	OrthogonalCamera();
	virtual ~OrthogonalCamera();

	/**
	 * \brief This moves the camera's view by the mouse movements (First person view)
	 *
	 * \param x The new mouse position
	 * \param y The new mouse position
	 */
	void set_view_by_mouse(float x, float y);

	void set_button_press_mouse(float x, float y);

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

	void set_ortho_axis(int axis);

private:
	float down_x_;
	float down_y_;
	float zoom_;
	int ortho_axis_;
};

#endif /* ORTHOGONAL_CAMERA_H_ */
