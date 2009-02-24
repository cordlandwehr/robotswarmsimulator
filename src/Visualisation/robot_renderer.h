/*
 * robot_renderer.h
 *
 *  Created on: 08.02.2009
 *      Author: kamil
 */

#ifndef ROBOT_RENDERER_H_
#define ROBOT_RENDERER_H_

#include <string>
#include <boost/smart_ptr.hpp>

#include "../Utilities/vector_arithmetics.h"


class SimulationRenderer;
class RobotData;


/**
 * This class renders robots and their properties like velocity, acceleration. etc.
 *
 * Before using the Class call the init() method.
 */
class RobotRenderer{

public:
		/**
		 * This robot renderer needs the SimulationRenderer which is uses this renderer.
		 */
		RobotRenderer(SimulationRenderer * renderer );
		~RobotRenderer();

		/**
		 * This method is used for initializing the renderer. It creates a compiled list
		 * for drawing the robots as spheres.
		 */
		void init();

		/**
		 * This draws a single robot. Make sure that all attributes which should be drawn
		 * are set. There are no checks for them. The extrapolated positions of the robots are used.
		 *
		 * \param robot the robot to draw.
		 * \param extrapolate The value for the extrapolating.
		 *
		 */
		void draw_robot(const boost::shared_ptr<RobotData> & robot, double extrapolate ) const;

		/**
		 * Sets the default color of the drawn robots.
		 */
		void set_default_color(float r, float g, float b, float alpha);

		/**
		 * This is used for loading the model for the robot.
		 */
		void load_model(std::string & filename );


private:

	void draw_robot_sphere(const boost::shared_ptr<Vector3d> & pos) const;

	/**
	 * The Simulationrenderer which uses this robot renderer.
	 */
	SimulationRenderer * renderer_;

	/**
	 * The default color for drawing spheres.
	 */
	float default_color_[4];

	/**
	 * Indicates whether models are used for drawing the robots
	 */
	bool use_model_;

	/**
	 * Holds the id of the compiled list with the sphere.
	 */
	unsigned int compiled_list_;

};


#endif /* ROBOT_RENDERER_H_ */
