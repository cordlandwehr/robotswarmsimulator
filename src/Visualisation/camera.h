/*
 * camera.h
 *
 *  Created on: 15.01.2009
 *      Author: kamil
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <vector>

#include <boost/smart_ptr.hpp>

#include "../Utilities/vector_arithmetics.h"


#define kSpeed	0.1f

class WorldObject;
class Obstacle;
class RobotData;


/**
 * This is the base class for all cameras.
 *
 * Usage:
 * 		Call update and look at the beginning of each frame.
 *
 * 		The look method can be split into rotation and translation for some effects, like a skybox.
 *
 */
class Camera {

public:

	/**
	 * The constructor
	 */
	Camera();

	/**
	 *
	 */
	virtual ~Camera();

	/**
	 *  \brief Sets the position of the camera, the point to look at and the world up vector.
	 *
	 *  \param position The position of the camera
	 *  \param view The point to look at.
	 *  \param up_vector The vector which specifies what is up in the world
	 */
	void position_camera(const Vector3d & position,const Vector3d &view, const Vector3d &up_vector);


	/**
	 * This rotates the camera's view around the position depending on the values passed in.
	 *
	 * \param angle The angle of rotation
	 * \param rot_vec The vector to rotate around
	 *
	 */
	virtual void rotate_view(float angle, const Vector3d & rot_vec);



	/**
	 *  \brief Handles the mouse input
	 *
	 *  \param x The new position in x direction
	 *  \param y The new position in y direction
	 */
	virtual void set_view_by_mouse(int x, int y);



	/**
	 * \brief This rotates the camera around a point
	 *
	 * \param center The center to rotated around
	 * \paramrot_vec The rotation vector to use for rotating.
	 */
	virtual void rotate_around_point(Vector3d center, Vector3d rot_vec);



	/**
	 * \brief This strafes the camera left or right depending on the speed (+/-)
	 *
	 * \param speed the speed for movement
	 */
	virtual void strafe_camera(float speed);


	/**
	 *  \brief This will move the camera forward or backward depending on the speed
	 * \param speed The speed for movement
	 */
	virtual void move_camera(float speed);



	/**
	 * This will move the camera forward
	 */
	virtual void move_forward();
	/**
	 * This will move the camera backward
	 */
	virtual void move_backward();

	virtual void move_up();

	virtual void move_down();
	/**
	 * This will strafe the camera to the left
	 */
	virtual void strafe_left();
	/**
	 * This will strafe the camera to the right
	 */
	virtual void strafe_right();

	/**
	 * Sets the information about the width of the screen.
	 * It should be updated when ever the window size changes.
	 */
	void set_screen_width(int width){
		screen_width_ = width;
	}

	/**
	 * Sets the information about the height of the screen.
	 * It should be updated when ever the window size changes.
	 */
	void set_screen_height(int height){
		screen_height_ = height;
		}

	/**
	 * This updates the camera's view and other data.
	 */
	virtual void update(const std::vector<boost::shared_ptr<WorldObject> > & marker,
						const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
						const std::vector<boost::shared_ptr<RobotData> >& robot_data,
						double extrapolate );


	/**
	 * \brief Does the rotating of the camera.
	 */
	virtual void look_rot() const;

	/**
	 * \brief does the translating of the camera
	 */
	virtual void look_translate() const;

	/**
	 * \brief This method lets the maera look at the view point from the position point
	 */
	virtual void look() const;


	/**
	 * Returns the position of the camera.
	 */
	Vector3d & position() {
		return position_;
	}

	/**
	 * Returns the view point of the camera.
	 */
	Vector3d & view()	{
		return view_;
	}

	/**
	 * Returns the vector which defines where "up" is in the world.
	 */
	Vector3d & up_vector() {	return up_vector_;		}

	/**
	 * The strafe vector. It is the right of the viewer.
	 */
	Vector3d & strafe()	{
		return strafe_;
	}

	/**
	 * Returns the saved window height.
	 */
	int screen_height(){
		return screen_height_;
	}

	/**
	 * Returns the saved window width.
	 */
	int screen_width(){
		return screen_width_;
	}


protected:

	// The camera's position

	Vector3d position_;

	// The camera's view

	Vector3d view_;

	// The camera's up vector

	Vector3d up_vector_;

	// The camera's strafe vector

	Vector3d strafe_;

	int screen_width_;

	int screen_height_;

};

Vector3d  Cross(const Vector3d & vector1 ,const  Vector3d & vector2);
float Magnitude(const Vector3d & vec);
Vector3d & Normalize(Vector3d & vector) ;


#endif /* CAMERA_H_ */
