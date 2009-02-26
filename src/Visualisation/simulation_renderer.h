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
#include "../Visualisation/follow_swarm_camera.h"
#include "../Visualisation/moveable_camera.h"
#include "../SimulationControl/visualizer.h"
#include "texture.h"
#include "sky_box.h"

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

	SimulationRenderer();

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

    /**
     * These functions handle user input
     * Two functions for keyboard input are necessary as special keys and ASCII character encodings overlap (example: d=100='left')
     */
    void mouse_func(int button, int state, int x, int y);
    void keyboard_func(unsigned char key, int x, int y);
    void keyboard_special_func(int key, int x, int y);

    /**
     * \brief This Method draws the whole szene.
     *
     * The Method draws the whole scene given by an WorldInformation Object and
     * an extraplolating time.
     *
     * \param world_info The WorldInformation Object which describes the current world
     * \param extrapolate The value to use for extrapolating.
     */
	void draw(double extrapolate, const boost::shared_ptr<WorldInformation> & world_info);

	/**
	 * \brief This Method provides access to the camera Object.
	 *
	 * \return The used Camera Object.
	 */
	const boost::shared_ptr<Camera> & camera(){ return cameras_[active_camera_index_]; }

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

	void set_render_cog(bool render){
		render_cog_ = render;
	}

	bool render_cog(){
		return render_cog_;
	}

	void switch_render_cog(){
		render_cog_ = !render_cog_;
	}

	const Vector3d & cog(){
		return cog_;
	}

	void set_render_coord_system(bool render){
		render_coord_system_ = render;
	}

	bool render_coord_system(){
		return render_coord_system_;
	}

	void switch_render_coord_system(){
		render_coord_system_ = !render_coord_system_;
	}

	void set_render_local_coord_system( bool render ){
		render_local_coord_system_ = render;
	}

	void switch_render_local_coord_system(){
		render_local_coord_system_ = !render_local_coord_system_;
	}

	bool render_local_coord_system(){
		return render_local_coord_system_ ;
	}

	void set_render_velocity(bool  render ){
		render_velocity_ = render;
	}

	void switch_render_velocity(){
		render_velocity_ = !render_velocity_;
	}

	bool render_velocity(){
		return render_velocity_;
	}

	void set_render_acceleration( bool render ){
		render_acceleration_ = render;
	}

	void switch_render_acceleration(){
		render_acceleration_ = !render_acceleration_;
	}
	bool render_acceleration(){
		return render_acceleration_;
	}

private:
	void draw_line(Vector3d pos1, Vector3d pos2, const float* color);
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

	void draw_help();

	void draw_about();

	void draw_cog(const boost::shared_ptr<WorldInformation> world_info );

	void draw_coord_system();

	void setup_projection();

	enum ProjectionType {
		PROJ_PERSP,
		PROJ_ORTHO
	};

	ProjectionType projection_type_  ;

	boost::scoped_ptr<SkyBox> sky_box_;

	/**
	 * A helper function to draw text
	 */
	int font_bitmap_string(const std::string & str);

	unsigned int active_camera_index_;

	boost::array<boost::shared_ptr<Camera>,2> cameras_;

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
	 *
	 */
	bool render_cog_;

	/**
	 *
	 */
	Vector3d cog_;

	/**
	 *
	 */
	bool  render_coord_system_;
	/**
	 *
	 */
	bool render_local_coord_system_;

	/**
	 *
	 */
	bool render_acceleration_;
	
	/**
	 *
	 */
	bool render_velocity_;
	
	bool render_help_;

	bool render_about_;

	/**
	 * RobotRenderer
	 */
	boost::shared_ptr<RobotRenderer> robot_renderer_;
	Texture tex_;

#if !defined(__linux__) && !defined(__APPLE__)
	void (APIENTRY *p_glWindowPos2i)(int , int) ;
#endif
};


#endif /* SIMULATION_RENDERER_H_ */
