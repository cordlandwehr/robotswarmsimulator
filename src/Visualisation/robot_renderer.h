/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>

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
		RobotRenderer(SimulationRenderer* renderer );
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
		void draw_robot(const boost::shared_ptr<RobotData>& robot ) const;

		/**
		 * Sets the default color of the drawn robots.
		 */
		void set_default_color(float r, float g, float b, float alpha);

// 		void set_extrapolate(double ext) {
// 			extrapolate_ = ext;
// 		}

		void set_robot_size(double s) { robot_size_ = s; }
private:

	void draw_robot_sphere(const boost::shared_ptr<Vector3d> & pos, double size) const;

	double robot_size_;

	/**
	 * The Simulationrenderer which uses this robot renderer.
	 */
	SimulationRenderer * renderer_;

	/**
	 * The default color for drawing spheres.
	 */
	float default_color_[4];

	/**
	 * Holds the id of the compiled list with the sphere.
	 */
	unsigned int compiled_list_;

/*	double extrapolate_;*/
};

#endif /* ROBOT_RENDERER_H_ */
