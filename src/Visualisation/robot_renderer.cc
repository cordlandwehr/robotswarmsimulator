/*
	This file is part of RobotSwarmSimulator.

	Copyright (C) 2009-2010  Christoph Raupach <craupach@gmail.com>
	Copyright (C) 2009-2010  Kamil Swierkot <kamil@campus.uni-paderborn.de>
	Copyright (C) 2009-2010  Alexander Klaas <aklaas@uni-paderborn.de>
	Copyright (C) 2009-2010  Peter Kling <kronos@uni-paderborn.de>

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
 * robot_renderer.cc
 *
 *  Created on: 08.02.2009
 *      Author: kamil
 */

#include <cmath>
#include <cstdio>

#include <boost/tuple/tuple.hpp>

#include "../OpenGL/gl_headers.h"
#include "../OpenGL/glu_headers.h"
#include "../OpenGL/pg_glut.h"

#include "../Model/robot_data.h"

#include "camera.h"
#include "simulation_renderer.h"
#include "robot_renderer.h"


namespace {

// constants for robot drawing

// values for spheres
const float kRobotRadius = 0.13;
const int kRobotSlices = 13;
const int kRobotStacks = 13;

// colors
const float kRobotColor[] = {0.0f,1.0f,0.0f,1.0f};
const float kVelColor[] = {1.0f,0.5f,0.0f,1.0f};
const float kAccelColor[] = {0.0f,0.5f,1.0f,1.0f};

const float kCogColor[] = {1.0f,1.0f,0.0f,1.0f};

const float kCoordXColor[] = {1.0f,0.0f,0.0f,1.0f};
const float kCoordYColor[] = {0.0f,1.0f,0.0f,1.0f};
const float kCoordZColor[] = {0.0f,0.0f,1.0f,1.0f};

// scale factors
const float kScaleVecs = 2.0;
const float kScaleCoordVecsRev = 2.0;

}

RobotRenderer::RobotRenderer(SimulationRenderer * renderer): robot_size_(1.0){
	renderer_ = renderer;
	for (int i=0; i < 4; i++ ){
		default_color_[i] = kRobotColor[i];
	}
}

RobotRenderer::~RobotRenderer() {
	//Nothing to do atm
}

void RobotRenderer::init() {
	compiled_list_ = glGenLists(1);

	glNewList( compiled_list_, GL_COMPILE );
		PgGLUT::glutSolidSphere(kRobotRadius, kRobotSlices, kRobotStacks);
	glEndList();
}

void RobotRenderer::draw_robot(const boost::shared_ptr<RobotData> & robot ) const{
	boost::shared_ptr<Vector3d> rob_pos = robot->extrapolated_position();

	if (renderer_->render_cog()) {
		glBegin(GL_LINES);
			glColor3fv(kCogColor);
			glVertex3f(renderer_->cog()(0), renderer_->cog()(1), renderer_->cog()(2) );
			glVertex3f((*rob_pos)(0), (*rob_pos)(1), (*rob_pos) (2) );
		glEnd();
	}


	glPushMatrix();
		glTranslatef((*rob_pos)(0), (*rob_pos)(1), (*rob_pos)(2) );

	glBegin(GL_LINES);
// 	if (renderer_->render_velocity()){
// 
// 		Vector3d vel = *robot->extrapolated_velocity(extrapolate_);
// 		glColor3fv(kVelColor);
// 		glVertex3f(0 ,0, 0 );
// 		glVertex3f( kScaleVecs * vel(0),
// 				    kScaleVecs *  vel(1),
// 					kScaleVecs *  vel(2) );
// 
// 	}

// 	if (renderer_->render_acceleration()){
// 
// 		Vector3d acc = robot->acceleration();
// 		glColor3fv(kAccelColor);
// 		glVertex3f(0 , 0, 0 );
// 		glVertex3f( kScaleVecs * acc(0),
// 					kScaleVecs * acc(1),
// 					kScaleVecs * acc(2) );
// 	}

	if (renderer_->render_local_coord_system()){

		boost::tuple<boost::shared_ptr<Vector3d>,
					 boost::shared_ptr<Vector3d>,
					 boost::shared_ptr<Vector3d> > local_coord = robot->coordinate_system_axis();

		boost::shared_ptr<Vector3d> x_coord = local_coord.get<0>();
		boost::shared_ptr<Vector3d> y_coord = local_coord.get<1>();
		boost::shared_ptr<Vector3d> z_coord = local_coord.get<2>();


		glColor3fv(kCoordXColor);
		glVertex3f(0, 0, 0);
		glVertex3f( (*x_coord)(0) ,
					(*x_coord)(1) ,
					(*x_coord)(2)  );

		glColor3fv(kCoordYColor);
		glVertex3f(0, 0,0 );
		glVertex3f((*y_coord)(0) ,
				   (*y_coord)(1) ,
				   (*y_coord)(2)  );

		glColor3fv(kCoordZColor);
		glVertex3f(0, 0, 0 );
		glVertex3f( (*z_coord)(0) ,
					(*z_coord)(1) ,
					(*z_coord)(2) );


	}

	glEnd();

	unsigned int rob_color =  robot->color() < kRobotIdColorNum ? robot->color() : 0 ;
	if(robot->marker_information().has_key("color")) {
		boost::any color = robot->marker_information().get_data("color");
		double * color_ptr = boost::any_cast<double>(&color);
		if(color_ptr)
			rob_color = *color_ptr < kRobotIdColorNum ? static_cast<unsigned int>(*color_ptr) : 0 ;
	}
	glColor3fv(&kRobotIdColor[rob_color][0]);
	draw_robot_sphere( rob_pos );

	glPopMatrix();
}

void RobotRenderer::set_default_color(float r, float g, float b, float alpha){
	default_color_[0] = r;
	default_color_[1] = g;
	default_color_[2] = b;
	default_color_[3] = alpha;
}

inline void RobotRenderer::draw_robot_sphere(const boost::shared_ptr<Vector3d> & pos) const{
	glScalef(robot_size_, robot_size_, robot_size_);
	glCallList( compiled_list_ );
}


