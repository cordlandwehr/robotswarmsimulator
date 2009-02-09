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

class RobotRenderer{

public:
		RobotRenderer(SimulationRenderer * renderer );
		~RobotRenderer();

		void draw_robot(const boost::shared_ptr<RobotData> & robot, double extrapolate ) const;
		void set_default_color(float r, float g, float b, float alpha);

		void load_model(std::string & filename );


private:

	void draw_robot_sphere(const boost::shared_ptr<Vector3d> & pos) const;

	SimulationRenderer * renderer_;
	float default_color_[4];
	bool use_model_;

};


#endif /* ROBOT_RENDERER_H_ */
