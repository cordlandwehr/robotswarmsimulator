/*
 * cog_camera.h
 *
 *  Created on: 02.03.2009
 *      Author: kamil
 */

#ifndef COG_CAMERA_H_
#define COG_CAMERA_H_

#include <vector>

#include <boost/smart_ptr.hpp>

#include "../Utilities/vector_arithmetics.h"

class WorldObject;
class Obstacle;
class RobotData;

#include "camera.h"

class CogCamera : public Camera {

  public:
	CogCamera();
	~CogCamera(){};

	/**
	 * Lets the camera move left or right on the sphere
	 */
	void strafe_camera(float speed);

	/**
	 *  \brief This will move the camera forward or backward depending on the speed
	 * \param speed The speed for movement
	 */
	void move_camera(float speed);

	/**
	 *  Moves the camera up or down depending on the speed
	 */
	void move_camera_up_down(float speed);

	/**
	 * lets the camera move "up" on the sphere.
	 */
	void move_up();

	/**
	* Lets the camera moce "down" on the sphere.
	*/
	void move_down();

	/**
	 * Moves the camera forward.
	 */
	void move_forward();

	/**
	 * This will move the camera backward.
	 */
	void move_backward();

	/**
	 * This will strafe the camera to the left.
	 */
	void strafe_left();

	/**
	 * This will strafe the camera to the right.
	 */
	void strafe_right();

	/**
	 * This updates the camera's view and other data.
	 */
	void update(const std::vector<boost::shared_ptr<WorldObject> > & marker,
			const std::vector<boost::shared_ptr<Obstacle> >& obstacles,
			const std::vector<boost::shared_ptr<RobotData> >& robot_data,
			double extrapolate );

	void set_init_pos(Vector3d & pos);

	/**
	 * Returns the name of the camera type.
	 */
	std::string get_name();

  private:

	  /**
	   * calculates the sphere vec.
	   */
	  void calc_sphere_vec();

	  /**
	   * sets the rotation value for left / right
	   */
	  void set_rot_phi(double phi);

	  /**
	   * sets the rotation value for down/up
	   */
	  void set_rot_theta(double theta);

	  double rot_phi_;
	  double rot_theta_;

	  double radius_;

	  Vector3d sphere_vec_;
	  Vector3d init_pos_;
	  bool need_init_pos_;
};

#endif /* COG_CAMERA_H_ */
