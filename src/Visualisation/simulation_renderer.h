/*
 * simulation_renderer.h
 *
 *  Created on: 18.01.2009
 *      Author: kamil
 */

#ifndef SIMULATION_RENDERER_H_
#define SIMULATION_RENDERER_H_

#include <vector>
#include <string>

#include <boost/smart_ptr.hpp>

#include "../Utilities/vector3d.h"

class WorldInformation;
class WorldObject;
class Obstacle;
class Box;
class Sphere;
class RobotData;
class Camera;



class SimulationRenderer{

public:

	SimulationRenderer(boost::shared_ptr<Camera> & camera);

	/**
	 * \brief Initializes the Simulation Renderer
	 *
	 * This Method has to be called before rendering inorder to set up OpenGL.
	 *
	 * \param width the width of the window.
	 * \param height The height of the window.
	 */
	void init(int width, int height);

	/**
	 * \brief Resizes the OpenGL settings for a new window size.
	 *
	 * This method has to be called from the function connected to the glutReshapeFunc.
	 *
	 * \param width The new width of the window.
	 * \param height The new height of the window.
	 */
    void resize(int width, int height);

    void mouse_func(unsigned int type, int x, int y);
    void keyboard_func(unsigned int key, int x, int y);

    /**
     * \brief This Method draws the whole szene.
     *
     * The Method draws the whole scene given by an WorldInformation Object and
     * an extraplolating time.
     *
     * \param world_info The WorldInformation Object which describes the current world
     * \param extrapolate The value to use for extrapolating.
     */
	void draw(const boost::shared_ptr<WorldInformation> & world_info, double extrapolate);

	/**
	 * \brief Used to reset the camera.
	 *
	 * \param new_camera The new Camera Object.
	 */
	void set_camera(boost::shared_ptr<Camera> & new_camera);

	/**
	 * \brief This Method provides access to the camera Object.
	 *
	 * \return The used Camera Object.
	 */
	const boost::shared_ptr<Camera> & camera(){ return camera_;}

	/**
	 * \brief Sets the new color of text in RGBA
	 *
	 * \param r red value
	 * \param g green value
	 * \param b blue value
	 * \param alpha alpha value
	 *
	 */
	void set_text_color(float r, float g ,float b, float alpha);

	/**
	 * \brief Sets the new color of obstacles in RGBA
	 *
	 * \param r red value
	 * \param g green value
	 * \param b blue value
	 * \param alpha alpha value
	 *
	 */
	void set_obstacle_color(float r, float g ,float b, float alpha);

	/**
	 * \brief Sets the new color of robots in RGBA
	 *
	 * \param r red value
	 * \param g green value
	 * \param b blue value
	 * \param alpha alpha value
	 *
	 */
	void set_robot_color(float r, float g ,float b, float alpha);

	/**
	 * \brief Sets the new color of markers in RGBA
	 *
	 * \param r red value
	 * \param g green value
	 * \param b blue value
	 * \param alpha alpha value
	 *
	 */
	void set_marker_color(float r, float g ,float b, float alpha);

	/**
	 * \brief Sets whether robot id should be displayed or not.
	 *
	 * \param render The boolean flag
	 */
	void set_render_robot_ids(bool render){
		render_robot_ids_ = render;
	}

	/**
	 * \brief Returns whether the ids are drawn.
	 *
	 */
	bool render_robot_ids(){
		return render_robot_ids_;
	}

private:

	void draw_obstacle(const boost::shared_ptr<Obstacle> & obstacle);
	void draw_box(const Box*  box);
	void draw_sphere(const Sphere*  sphere);
	void draw_robot(const boost::shared_ptr<RobotData> & robot);

	void draw_marker(const boost::shared_ptr<WorldObject> & marker);

	void draw_text2d(int x, int y, const std::string &str);
	void draw_text3d(const Vector3d & vector, const std::string &str );

	int font_bitmap_string(const std::string & str);

	boost::shared_ptr<Camera> camera_;

	int screen_width_;
	int screen_height_;

	float text_color_[4];
	float obstacle_color_[4];
	float robot_color_[4];
	float marker_color_[4];

	bool render_robot_ids_;


	bool use_mouse_;

	double extrapolate_;

};


#endif /* SIMULATION_RENDERER_H_ */
