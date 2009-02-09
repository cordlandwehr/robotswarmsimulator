/*
 * robot_renderer.cc
 *
 *  Created on: 08.02.2009
 *      Author: kamil
 */

#include <cmath>

#include "../OpenGL/GLHeaders.h"
#include "../OpenGL/GLUHeaders.h"
#include "../OpenGL/GLUTHeaders.h"

#include "../Model/robot_data.h"

#include "simulation_renderer.h"
#include "robot_renderer.h"

const float kRobotRadius = 1.0;
const int kRobotSlices = 30;
const int kRobotStacks = 30;

const float kRobotColor[] = {0.0f,1.0f,0.0f,1.0f};






RobotRenderer::RobotRenderer(SimulationRenderer * renderer ){


	renderer_ = renderer;
	use_model_ = false;
	for(int i=0; i < 4; i++ ){
		default_color_[i] = kRobotColor[i];
	}

}

RobotRenderer::~RobotRenderer(){
	//Nothing to do atm
}

void RobotRenderer::draw_robot(const boost::shared_ptr<RobotData> & robot, double extrapolate ) const{

	boost::shared_ptr<Vector3d> rob_pos = robot->extrapolated_position(extrapolate);
	if(!use_model_){
		draw_robot_sphere( rob_pos );
	}
}

void RobotRenderer::set_default_color(float r, float g, float b, float alpha){

	default_color_[0] = r;
	default_color_[1] = g;
	default_color_[2] = b;
	default_color_[3] = alpha;

}

void RobotRenderer::load_model(std::string & filename ){

	//TODO(kamil) implement this...

}



void RobotRenderer::draw_robot_sphere(const boost::shared_ptr<Vector3d> & pos) const{


		glPushMatrix();
			glTranslatef((*pos)(0), (*pos)(1), (*pos)(2) );
			glColor3f(default_color_[0], default_color_[1], default_color_[2]);
			glutSolidSphere(kRobotRadius, kRobotSlices, kRobotStacks);
		glPopMatrix();


}


