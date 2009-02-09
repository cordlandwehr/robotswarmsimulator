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


#include "../Utilities/vector_arithmetics.h"
#include "../Visualisation/simulation_renderer.h"
#include "../Visualisation/camera.h"
#include "../SimulationControl/visualizer.h"

class WorldInformation;
class WorldObject;
class Obstacle;
class Box;
class Sphere;
class RobotData;
class Camera;
class RobotRenderer;


class SimulationRenderer : public Visualizer {

public:

	SimulationRenderer(boost::shared_ptr<Camera> & camera);

	void init();

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

    void mouse_func(int button, int state, int x, int y);
    void keyboard_func(unsigned char key, int x, int y);

    /**
     * \brief This Method draws the whole szene.
     *
     * The Method draws the whole scene given by an WorldInformation Object and
     * an extraplolating time.
     *
     * \param world_info The WorldInformation Object which describes the current world
     * \param extrapolate The value to use for extrapolating.
     */
	void draw(double extrapolate, boost::shared_ptr<WorldInformation> world_info);

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

	/**
	 * Draws an obstacle. It Determines the type of the obstacle
	 * and calls the corresponding method.
	 *
	 * \param obstacle The Obstacle to draw
	 */
	void draw_obstacle(const boost::shared_ptr<Obstacle> & obstacle);

	/**
	 * Draws a box.
	 * \param box the box
	 */
	void draw_box(const Box*  box);

	/**
	 * Draws a sphere.
	 * \param sphere The sphere
	 */
	void draw_sphere(const Sphere*  sphere);

	/**
	 * Draws a robot.
	 */
	void draw_robot(const boost::shared_ptr<RobotData> & robot);

	/**
	 * Draws a marker.
	 *
	 * \param marker The marker to draw
	 */
	void draw_marker(const boost::shared_ptr<WorldObject> & marker);

	void draw_text2d(int x, int y, const std::string &str);
	void draw_text3d(const Vector3d & vector, const std::string &str );

	/**
	 * A helper function to draw text
	 */
	int font_bitmap_string(const std::string & str);

	boost::shared_ptr<Camera> camera_;

	/**
	 * The width of the screen
	 */
	int screen_width_;

	/**
	 * The height of the screen
	 */
	int screen_height_;

	/**
	 * The color of drawn text in RGBA
	 */
	float text_color_[4];

	/**
	 * The color of drawn obstacles in RGBA
	 */
	float obstacle_color_[4];

	/**
	 * The color of drawn robots in RGBA
	 */
	float robot_color_[4];

	/**
	 * The color of drawn markers in RGBA
	 */
	float marker_color_[4];

	/**
	 * Decides whether Robots Id get drawn or not
	 */
	bool render_robot_ids_;

	/**
	 * Decides whether the mouse function should do anything
	 */
	bool use_mouse_;

	/**
	 * The value for extrapolating
	 */
	double extrapolate_;

	/**
	 * RobotRenderer
	 */
	boost::shared_ptr<RobotRenderer> robot_renderer_;

};


#endif /* SIMULATION_RENDERER_H_ */
